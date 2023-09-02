#pragma once

#include "kat/engine.hpp"
#include "kat/util/interfaces.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace kat::util {
    class Camera : public IPositionable<glm::vec3> {
    public:

        Camera();
        virtual ~Camera();

        glm::mat4 getView();
        glm::mat4 getProjection();
        glm::mat4 getCombined();

        virtual void update();
        virtual void updateView();
        virtual void updateProjection() = 0;

    protected:

        glm::mat4 m_Projection = glm::identity<glm::mat4>();
        glm::mat4 m_View = glm::identity<glm::mat4>();
        glm::mat4 m_Combined = glm::identity<glm::mat4>();
    };

    class OrthographicCamera : public Camera {
    public:

        OrthographicCamera(float left, float right, float bottom, float top);


        void setZoom(float zoom);
        [[nodiscard]] float getZoom() const noexcept;
        void addZoom(float dzoom);

        void setZoomBounds(float minPerc, float maxPerc);
        [[nodiscard]] float getZoomScale() const noexcept;

        void updateProjection() override;

    private:
        float m_Left, m_Right, m_Bottom, m_Top;
        float m_Zoom;
        float m_MinZoom = -INFINITY, m_MaxZoom = INFINITY;
        float m_ZoomScale;
    };
}