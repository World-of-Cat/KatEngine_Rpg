#include "camera.hpp"

namespace kat::util {
    Camera::Camera() = default;
    Camera::~Camera() = default;

    glm::mat4 Camera::getView() {
        return m_View;
    }

    glm::mat4 Camera::getProjection() {
        return m_Projection;
    }

    glm::mat4 Camera::getCombined() {
        return m_Combined;
    }

    void Camera::update() {
        m_View = glm::identity<glm::mat4>();
        updateView();
        updateProjection();
        m_Combined = m_Projection * m_View;
    }

    void Camera::updateView() {
        m_View = glm::translate(m_View, -m_Position);
    }

    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
                                           : m_Left(left), m_Right(right), m_Bottom(bottom), m_Top(top), m_Zoom(0.0f) {
        m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top);
    }

    void OrthographicCamera::updateProjection() {
        m_Projection = glm::ortho(m_Left, m_Right, m_Bottom, m_Top);
        m_ZoomScale = expf(m_Zoom);
    }

    void OrthographicCamera::setZoom(float zoom) {
        m_Zoom = std::clamp(zoom, m_MinZoom, m_MaxZoom);
    }

    float OrthographicCamera::getZoom() const noexcept {
        return m_Zoom;
    }

    void OrthographicCamera::addZoom(float dzoom) {
        setZoom(m_Zoom + dzoom);
    }

    void OrthographicCamera::setZoomBounds(float minPerc, float maxPerc) {
        m_MinZoom = minPerc;
        m_MaxZoom = maxPerc;
    }

    float OrthographicCamera::getZoomScale() const noexcept {
        return m_ZoomScale;
    }
}