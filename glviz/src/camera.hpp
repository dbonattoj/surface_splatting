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

#ifndef CAMERA_HPP
#define CAMERA_HPP

#include <Eigen/Dense>

#include "../src/trackball.hpp"

namespace GLviz
{

class Frustum
{

public:
    float& left() { return m_left; }
    float& right() { return m_right; }

    float& bottom() { return m_bottom; }
    float& top() { return m_top; }

    float& near_() { return m_near; }
    float& far_() { return m_far; }

private:
    float m_left, m_right;
    float m_bottom, m_top;
    float m_near, m_far;
};


struct Plane {
    Eigen::Vector3f normal;
    float distance;
};

class Camera {
public:
    Camera();

    Eigen::Matrix4f const& get_model_matrix() const;
    Eigen::Matrix4f const& get_view_matrix() const;
    Eigen::Matrix4f const& get_projection_matrix() const;
    Eigen::Vector3f const& get_position_offset() const;

    void set_model_matrix(Eigen::Matrix4f const& model);
    void set_view_matrix(Eigen::Matrix4f const& view);
    void set_projection_matrix(Eigen::Matrix4f const& projection);
    void set_position_offset(Eigen::Vector3f const& position);


protected:
    Eigen::Matrix4f m_model_matrix;
    Eigen::Matrix4f m_view_matrix;
    Eigen::Matrix4f m_projection_matrix;
    Eigen::Vector3f m_position_offset;
};

class Scene_Camera : public Camera
{

public:
    Scene_Camera();
    virtual ~Scene_Camera();

    Frustum const& get_frustum() const;

    void set_frustum(Frustum const& frustum);
    void set_aspect(float aspect);
    void set_perspective(float fovy, float aspect, float near_,
        float far_);

    void set_position(Eigen::Vector3f const& position);

    void set_orientation(Eigen::Matrix3f const& orientation);
    void set_orientation(Eigen::Quaternionf const& orientation);

    void rotate(Eigen::Quaternionf const& rotation);
    void rotate(Eigen::Matrix3f const& rotation);

    void translate(Eigen::Vector3f const& translation);

    void trackball_begin_motion(float begin_x, float begin_y);

    void trackball_end_motion_rotate(float end_x, float end_y);
    void trackball_end_motion_zoom(float end_x, float end_y);
    void trackball_end_motion_translate(float end_x, float end_y);


    void set_frustum_from_modelViewProjection_matrix(Eigen::Matrix4f const & MVP);

protected:
    void set_projection_matrix_from_frustum();
    void compute_frustum_planes_from_model_view_projection_matrix(Eigen::Matrix4f const & MVP, Plane & left, Plane & right, Plane & top, Plane & bottom, Plane & near_, Plane &far_);
    Eigen::Vector3f intersection_plane_plane_plane(Plane const & p1, Plane const & p2, Plane const & p3);
    void set_modelview_matrix_from_orientation();

protected:
    Eigen::Vector3f m_position;
    Eigen::Quaternionf m_orientation;

    Frustum m_frustum;
    float m_fovy_rad;
    float m_aspect;

    float m_begin_x, m_begin_y;

    Trackball m_trackball;
};

}

#endif // CAMERA_HPP
