// This file is part of GLviz.
//
// Copyright(c) 2014, 2015 Sebastian Lipponer
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#include "camera.hpp"

#define M_PI 3.14159265359

using namespace Eigen;

namespace GLviz
{

    Matrix4f const&
        Camera::get_model_matrix() const
    {
        return m_model_matrix;
    }

    Matrix4f const&
        Camera::get_view_matrix() const
    {
        return m_view_matrix;
    }


    Matrix4f const&
        Camera::get_projection_matrix() const
    {
        return m_projection_matrix;
    }

    Eigen::Vector3f const & Camera::get_position_offset() const
    {
        return m_position_offset;
    }

    void Camera::set_model_matrix(Eigen::Matrix4f const & model)
    {
        m_model_matrix = model;
    }

    void Camera::set_view_matrix(Eigen::Matrix4f const & view)
    {
        m_view_matrix = view;
    }

    void Camera::set_projection_matrix(Eigen::Matrix4f const & projection)
    {
        m_projection_matrix = projection;
    }

    void Camera::set_position_offset(Eigen::Vector3f const & position)
    {
        m_position_offset = position;
    }


Scene_Camera::Scene_Camera()
    : m_position(Vector3f::Zero()),
      m_orientation(Quaternionf::Identity())
{
    set_perspective(60.0f, 4.0f / 3.0f, 0.25f, 10.0f);
}

Scene_Camera::~Scene_Camera()
{
}

Frustum const&
Scene_Camera::get_frustum() const
{
    return m_frustum;
}

void
Scene_Camera::set_projection_matrix_from_frustum()
{
    const float l = m_frustum.left();
    const float r = m_frustum.right();

    const float b = m_frustum.bottom();
    const float t = m_frustum.top();

    const float n = m_frustum.near_();
    const float f = m_frustum.far_();

    m_projection_matrix <<
        (2.0f * n) / (r - l), 0.0f, (r + l) / (r - l), 0.0f,
        0.0f, (2.0f * n) / (t - b), (t + b) / (t - b), 0.0f,
        0.0f, 0.0f, -(f + n) / (f - n), -(2.0f * n * f) / (f - n),
        0.0f, 0.0f, -1.0f, 0.0f;
}

void
Scene_Camera::set_frustum_from_modelViewProjection_matrix(Eigen::Matrix4f const &MVP) {
    
    Plane pL, pR, pB, pT, pN, pF;
    compute_frustum_planes_from_model_view_projection_matrix(MVP, pL, pR, pT, pB, pN, pF);

    m_frustum.left() = intersection_plane_plane_plane(pN, pL, pB)[0];
    m_frustum.right() = intersection_plane_plane_plane(pN, pR, pB)[0];

    m_frustum.top() = intersection_plane_plane_plane(pN, pL, pT)[2];
    m_frustum.bottom() = intersection_plane_plane_plane(pN, pL, pB)[2];

    m_frustum.near_() = intersection_plane_plane_plane(pN, pL, pB)[1];
    m_frustum.far_() = intersection_plane_plane_plane(pF, pL, pB)[1];

}

void Scene_Camera::compute_frustum_planes_from_model_view_projection_matrix(Eigen::Matrix4f const &MVP, Plane &left, Plane &right, Plane &top, Plane &bottom, Plane &near_, Plane &far_) {
    Plane &L = left;
    Plane &R = right;
    Plane &T = top;
    Plane &B = bottom;
    Plane &N = near_;
    Plane &F = far_;

    L.normal[0] = MVP(0, 0) + MVP(3, 0);
    L.normal[1] = MVP(0, 1) + MVP(3, 1);
    L.normal[2] = MVP(0, 2) + MVP(3, 2);
    L.distance = MVP(0, 3) + MVP(3, 3);

    R.normal[0] = -MVP(0, 0) + MVP(3, 0);
    R.normal[1] = -MVP(0, 1) + MVP(3, 1);
    R.normal[2] = -MVP(0, 2) + MVP(3, 2);
    R.distance = -MVP(0, 3) + MVP(3, 3);


    B.normal[0] = MVP(1, 0) + MVP(3, 0);
    B.normal[1] = MVP(1, 1) + MVP(3, 1);
    B.normal[2] = MVP(1, 2) + MVP(3, 2);
    B.distance = MVP(1, 3) + MVP(3, 3);

    T.normal[0] = -MVP(1, 0) + MVP(3, 0);
    T.normal[1] = -MVP(1, 1) + MVP(3, 1);
    T.normal[2] = -MVP(1, 2) + MVP(3, 2);
    T.distance = -MVP(1, 3) + MVP(3, 3);


    N.normal[0] = MVP(2, 0) + MVP(3, 0);
    N.normal[1] = MVP(2, 1) + MVP(3, 1);
    N.normal[2] = MVP(2, 2) + MVP(3, 2);
    N.distance = MVP(2, 3) + MVP(3, 3);

    F.normal[0] = -MVP(2, 0) + MVP(3, 0);
    F.normal[1] = -MVP(2, 1) + MVP(3, 1);
    F.normal[2] = -MVP(2, 2) + MVP(3, 2);
    F.distance = -MVP(2, 3) + MVP(3, 3);
}

Eigen::Vector3f Scene_Camera::intersection_plane_plane_plane(Plane const & p1, Plane const & p2, Plane const &p3) {

    Eigen::Matrix3f matrix_of_planes_normal;
    for (size_t comp = 0; comp < 3; ++comp)
        matrix_of_planes_normal(0, comp) = p1.normal[comp];

    for (size_t comp = 0; comp < 3; ++comp)
        matrix_of_planes_normal(1, comp) = p2.normal[comp];

    for (size_t comp = 0; comp < 3; ++comp)
        matrix_of_planes_normal(2, comp) = p3.normal[comp];

    const float det = matrix_of_planes_normal.determinant();

    if (det == 0) return Eigen::Vector3f();

    Eigen::Vector3f point = (p2.normal.cross(p3.normal)* (-p1.distance)
        + p3.normal.cross(p1.normal) * (-p2.distance)
        + p1.normal.cross(p2.normal) * (-p3.distance)) / det;

    return std::move(point);
}

void
Scene_Camera::set_modelview_matrix_from_orientation()
{
    Matrix3f dir = AngleAxisf(m_orientation).inverse().toRotationMatrix();

    m_model_matrix = Matrix4f::Identity();
    m_view_matrix = Matrix4f::Identity();

    Vector3f ori = dir * m_position;
   
    // Translation * Rotation
    m_view_matrix.topLeftCorner(3, 3) = dir;
    m_model_matrix.topRightCorner(3, 1) = m_position;
}

void
Scene_Camera::set_frustum(Frustum const& frustum)
{
    m_frustum = frustum;
    
    m_fovy_rad = 2.0f * std::atan(m_frustum.top() / m_frustum.near_());
    m_aspect = m_frustum.right() / m_frustum.top();

    set_projection_matrix_from_frustum();
}

void
Scene_Camera::set_perspective(float fovy, float aspect, float near_, float far_)
{
    m_fovy_rad = (static_cast<float>(M_PI) / 180.0f) * fovy;
    m_aspect = aspect;

    m_frustum.top() = std::tan(m_fovy_rad / 2.0f) * near_;
    m_frustum.bottom() = -m_frustum.top();
    m_frustum.right() = m_frustum.top() * m_aspect;
    m_frustum.left() = -m_frustum.right();
    m_frustum.near_() = near_;
    m_frustum.far_() = far_;

    set_projection_matrix_from_frustum();
}

void
Scene_Camera::set_aspect(float aspect)
{
    m_aspect = aspect;

    m_frustum.right() = m_frustum.top() * m_aspect;
    m_frustum.left() = -m_frustum.right();

    set_projection_matrix_from_frustum();
}

void
Scene_Camera::set_position(Vector3f const& position)
{
    m_position = position;

    set_modelview_matrix_from_orientation();
}

void
Scene_Camera::set_orientation(Matrix3f const& orientation)
{
    m_orientation = Quaternionf(orientation);
    m_orientation.normalize();

    set_modelview_matrix_from_orientation();
}

void
Scene_Camera::set_orientation(Quaternionf const& orientation)
{
    m_orientation = orientation;
    m_orientation.normalize();

    set_modelview_matrix_from_orientation();
}

void
Scene_Camera::rotate(Quaternionf const& rotation)
{
    Quaternionf ret = m_orientation * rotation;
    m_orientation = ret;
    m_orientation.normalize();
    
    set_modelview_matrix_from_orientation();
}

void
Scene_Camera::rotate(Matrix3f const& rotation)
{
    rotate(Quaternionf(rotation));
}

void
Scene_Camera::translate(Vector3f const& translation)
{
    m_position += translation;
    
    set_modelview_matrix_from_orientation();
}

void
Scene_Camera::trackball_begin_motion(float begin_x, float begin_y)
{
    m_begin_x = begin_x;
    m_begin_y = begin_y;
}

void
Scene_Camera::trackball_end_motion_rotate(float end_x, float end_y)
{
    float u0_x = 2.0f * m_begin_x - 1.0f;
    float u0_y = 1.0f - 2.0f * m_begin_y;

    float u1_x = 2.0f * end_x - 1.0f;
    float u1_y = 1.0f - 2.0f * end_y;

    rotate(m_trackball(u0_x, u0_y, u1_x, u1_y));

    trackball_begin_motion(end_x, end_y);
}

void
Scene_Camera::trackball_end_motion_zoom(float end_x, float end_y)
{
    float dy = end_y - m_begin_y;

    translate(Vector3f(0.0f, 0.0f, 2.0f * dy));

    trackball_begin_motion(end_x, end_y);
}

void Scene_Camera::trackball_end_motion_translate(float end_x, float end_y)
{
    float dx = end_x - m_begin_x;
    float dy = end_y - m_begin_y;

    translate(Vector3f(2.0f * dx, -2.0f * dy, 0.0f));

    trackball_begin_motion(end_x, end_y);
}

}
