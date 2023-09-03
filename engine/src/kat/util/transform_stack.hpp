#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <optional>

namespace kat::transform {

    namespace internal {
        struct stack_node {
            glm::mat4 matrix;
            glm::mat4 tailCombined;
            stack_node* tail;
        };

        inline stack_node* head;

        inline void push(glm::mat4 mat) {
            head = new stack_node{mat, head->tailCombined * head->matrix, head};
        };

        inline void pop() {
            auto* tail = head->tail;
            delete head;
            head = tail;
        };
    }

    inline void push(const glm::mat4& mat = glm::identity<glm::mat4>()) {
        internal::push(mat);
    };

    inline void pop() {
        internal::pop();
    };

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
}