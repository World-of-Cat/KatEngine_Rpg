#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

#include <optional>
#include <stack>

#ifdef KAT_LEAK_CHECKS
#define UNRAVEL_LEAK_SETUP size_t nLeaks = 0;
#define UNRAVEL_LEAK_COUNTER nLeaks++
#define UNRAVEL_LEAK_MSG if (nLeaks) spdlog::warn("[leak] {} transform stack levels leaking by unravel.", nLeaks);
#else
#define UNRAVEL_LEAK_SETUP
#define UNRAVEL_LEAK_COUNTER
#define UNRAVEL_LEAK_MSG
#endif

namespace kat::transform {

    namespace internal {
        struct stack_node {
            glm::mat4 matrix;
            glm::mat4 tailCombined;
            stack_node* tail;
        };

        inline stack_node* head;

        inline void push(glm::mat4 mat) {
            if (head) {
                head = new stack_node{mat, head->tailCombined * head->matrix, head};
            } else {
                head = new stack_node{mat, glm::identity<glm::mat4>(), nullptr};
            }
        };

        inline std::stack<stack_node*> markers;

        inline bool pop() {
            if (head) {
                if (!markers.empty() && markers.top() == head) markers.pop(); // internal will always pop, ignoring markers.
                auto *tail = head->tail;
                delete head;
                head = tail;
                return true;
            }
            return false;
        };

    }

    inline bool isMarked() {
        return internal::markers.empty();
    }

    inline bool isOnMarker() {
        return !internal::markers.empty() && internal::head == internal::markers.top();
    }

    inline void mark() {
        if (internal::head == nullptr) return;
        if (!isOnMarker()) internal::markers.push(internal::head);
    }

    inline bool unmark() {
        if (!internal::markers.empty()) {
            internal::markers.pop();
            return true;
        }
        return false;
    }

    inline void removeAllMarkers() {
        while (unmark());
    }

    inline void push(const glm::mat4& mat = glm::identity<glm::mat4>(), bool mark = false) {
        internal::push(mat);
        if (mark) kat::transform::mark();
    };

    // normally, its wanted behavior to pop through markers (the option is provided so that leak protection doesn't early pop transformations).
    inline bool pop(bool force=true) {
        if (force || !isOnMarker()) return internal::pop();
        return false;
    };

    // Simple memory leak protection. Generally preferred to stop at the first marker, however it makes sense to fully unravel at the end of the program.
    inline void unravel(bool ignoreMarkers=false) {
        UNRAVEL_LEAK_SETUP
        while (pop(ignoreMarkers)) UNRAVEL_LEAK_COUNTER;
        UNRAVEL_LEAK_MSG
    }


    inline glm::mat4 getTransform() {
        if (internal::head) {
            return internal::head->tailCombined * internal::head->matrix;
        }
        return glm::identity<glm::mat4>();
    };

    inline void apply(const glm::mat4& transformation) {
        internal::head->matrix *= transformation;
    };

    inline void resetHead() {
        internal::head->matrix = glm::identity<glm::mat4>();
    };

    inline void translate(const glm::vec3& translation) {
        apply(glm::translate(glm::identity<glm::mat4>(), translation));
    };

    inline void translate(const glm::vec2& translation) {
        translate(glm::vec3(translation, 0.0f));
    };

    inline void scale(const glm::vec3& scale) {
        apply(glm::scale(glm::identity<glm::mat4>(), scale));
    };

    inline void scale(const glm::vec2& scale_) {
        scale(glm::vec3(scale_, 1.0f));
    };

    inline void rotate(const glm::fquat& quat) {
        apply(glm::mat4_cast(quat));
    };

    inline void rotate(const glm::vec3& euler) {
        apply(glm::mat4_cast(glm::quat(euler)));
    };

    inline void rotate(float angle, const glm::vec3& axis) {
        apply(glm::mat4_cast(glm::angleAxis(angle, axis)));
    };

    inline void rotate2D(float angle) {
        rotate(angle, glm::vec3(0, 0, 1));

    };
}