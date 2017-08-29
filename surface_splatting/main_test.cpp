// This file is part of Surface Splatting.
//
// Copyright (C) 2010, 2015 by Sebastian Lipponer.
//
// Surface Splatting is free software: you can redistribute it and / or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Surface Splatting is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Surface Splatting. If not, see <http://www.gnu.org/licenses/>.

#include <GLviz>

#include "splat_renderer.hpp"

#include <Eigen/Core>

#include <iostream>
#include <memory>
#include <fstream>
#include <sstream>
#include <vector>
#include <array>
#include <exception>

using namespace Eigen;

namespace
{

GLviz::Camera camera;
std::unique_ptr<SplatRenderer> viz;

std::vector<Eigen::Vector3f>               m_ref_vertices;
std::vector<Eigen::Vector3f>               m_ref_normals;

std::vector<Eigen::Vector3f>               m_vertices;
std::vector<std::array<unsigned int, 3> >  m_faces;
std::vector<Eigen::Vector3f>               m_normals;

std::vector<Surfel>  m_surfels;

void load_triangle_mesh(std::string const& filename);

void
displayFunc()
{
    GLuint textureID = viz->render_frame(m_surfels);
#ifndef NDEBUG
	std::cout << "Texture ID: " << textureID << std::endl;
#endif
}

void
reshapeFunc(int width, int height)
{
    const float aspect = static_cast<float>(width) /
        static_cast<float>(height);

    glViewport(0, 0, width, height);
    camera.set_perspective(60.0f, aspect, 0.005f, 5.0f);

    viz->reshape(width, height);
}

void
closeFunc()
{
    viz = nullptr;
}

void
load_triangle_mesh(std::string const& filename)
{
    std::cout << "\nRead " << filename << "." << std::endl;
    std::ifstream input(filename);

    if (input.good()) {
        input.close();
        GLviz::load_raw(filename, m_vertices, m_faces);
    } else {
        throw std::runtime_error(std::string("Could not open mesh file! path = ") + filename);
    }

    std::cout << "  #vertices " << m_vertices.size() << std::endl;
    std::cout << "  #faces    " << m_faces.size() << std::endl;

    GLviz::set_vertex_normals_from_triangle_mesh(
        m_vertices, m_faces, m_normals);

    m_ref_vertices = m_vertices;
    m_ref_normals = m_normals;
}

void
load_plane(unsigned int n)
{
    const float d = 1.0f / static_cast<float>(2 * n);

    Surfel s(Vector3f::Zero(),
             2.0f * d * Vector3f::UnitX(),
             2.0f * d * Vector3f::UnitY(),
             Vector3f::Zero(),
             0);

    m_surfels.resize(4 * n * n);
    unsigned int m(0);

    for (unsigned int i(0); i <= 2 * n; ++i)
    {
        for (unsigned int j(0); j <= 2 * n; ++j)
        {
            unsigned int k(i * (2 * n + 1) + j);

            if (k % 2 == 1)
            {
                s.c = Vector3f(
                    -1.0f + 2.0f * d * static_cast<float>(j),
                    -1.0f + 2.0f * d * static_cast<float>(i),
                    0.0f);
                s.rgba = (((j / 2) % 2) == ((i / 2) % 2)) ? 0u : ~0u;
                m_surfels[m] = s;

                // Clip border surfels.
                if (j == 2 * n)
                {
                    m_surfels[m].p = Vector3f(-1.0f, 0.0f, 0.0f);
                    m_surfels[m].rgba = ~s.rgba;
                }
                else if (i == 2 * n)
                {
                    m_surfels[m].p = Vector3f(0.0f, -1.0f, 0.0f);
                    m_surfels[m].rgba = ~s.rgba;
                }
                else if (j == 0)
                {
                    m_surfels[m].p = Vector3f(1.0f, 0.0f, 0.0f);
                }
                else if (i == 0)
                {
                    m_surfels[m].p = Vector3f(0.0f, 1.0f, 0.0f);
                }
                else
                {
                    // Duplicate and clip inner surfels.
                    if (j % 2 == 0)
                    {
                        m_surfels[m].p = Vector3f(1.0, 0.0f, 0.0f);

                        m_surfels[++m] = s;
                        m_surfels[m].p = Vector3f(-1.0, 0.0f, 0.0f);
                        m_surfels[m].rgba = ~s.rgba;
                    }

                    if (i % 2 == 0)
                    {
                        m_surfels[m].p = Vector3f(0.0, 1.0f, 0.0f);

                        m_surfels[++m] = s;
                        m_surfels[m].p = Vector3f(0.0, -1.0f, 0.0f);
                        m_surfels[m].rgba = ~s.rgba;
                    }
                }

                ++m;
            }
        }
    }
}

void
load_cube()
{
    Surfel cube[24];
    unsigned int color = 0;

    // Front.
    cube[0].c  = Vector3f(-0.5f, 0.0f, 0.5f);
    cube[0].u = 0.5f * Vector3f::UnitX();
    cube[0].v = 0.5f * Vector3f::UnitY();
    cube[0].p = Vector3f(1.0f, 0.0f, 0.0f);
    cube[0].rgba  = color;

    cube[1]   = cube[0];
    cube[1].c = Vector3f(0.5f, 0.0f, 0.5f);
    cube[1].p = Vector3f(-1.0f, 0.0f, 0.0f);

    cube[2]   = cube[0];
    cube[2].c = Vector3f(0.0f, 0.5f, 0.5f);
    cube[2].p = Vector3f(0.0f, -1.0f, 0.0f);

    cube[3]   = cube[0];
    cube[3].c = Vector3f(0.0f, -0.5f, 0.5f);
    cube[3].p = Vector3f(0.0f, 1.0f, 0.0f);

    // Back.
    cube[4].c = Vector3f(-0.5f, 0.0f, -0.5f);
    cube[4].u = 0.5f * Vector3f::UnitX();
    cube[4].v = -0.5f * Vector3f::UnitY();
    cube[4].p = Vector3f(1.0f, 0.0f, 0.0f);
    cube[4].rgba = color;

    cube[5] = cube[4];
    cube[5].c = Vector3f(0.5f, 0.0f, -0.5f);
    cube[5].p = Vector3f(-1.0f, 0.0f, 0.0f);

    cube[6] = cube[4];
    cube[6].c = Vector3f(0.0f, 0.5f, -0.5f);
    cube[6].p = Vector3f(0.0f, 1.0f, 0.0f);

    cube[7] = cube[4];
    cube[7].c = Vector3f(0.0f, -0.5f, -0.5f);
    cube[7].p = Vector3f(0.0f, -1.0f, 0.0f);

    // Top.
    cube[8].c = Vector3f(-0.5f, 0.5f, 0.0f);
    cube[8].u = 0.5f * Vector3f::UnitX();
    cube[8].v = -0.5f * Vector3f::UnitZ();
    cube[8].p = Vector3f(1.0f, 0.0f, 0.0f);
    cube[8].rgba = color;

    cube[9]    = cube[8];
    cube[9].c  = Vector3f(0.5f, 0.5f, 0.0f);
    cube[9].p = Vector3f(-1.0f, 0.0f, 0.0f);

    cube[10]    = cube[8];
    cube[10].c  = Vector3f(0.0f, 0.5f, 0.5f);
    cube[10].p = Vector3f(0.0f, 1.0f, 0.0f);

    cube[11] = cube[8];
    cube[11].c = Vector3f(0.0f, 0.5f, -0.5f);
    cube[11].p = Vector3f(0.0f, -1.0f, 0.0f);

    // Bottom.
    cube[12].c = Vector3f(-0.5f, -0.5f, 0.0f);
    cube[12].u = 0.5f * Vector3f::UnitX();
    cube[12].v = 0.5f * Vector3f::UnitZ();
    cube[12].p = Vector3f(1.0f, 0.0f, 0.0f);
    cube[12].rgba = color;

    cube[13] = cube[12];
    cube[13].c = Vector3f(0.5f, -0.5f, 0.0f);
    cube[13].p = Vector3f(-1.0f, 0.0f, 0.0f);

    cube[14] = cube[12];
    cube[14].c = Vector3f(0.0f, -0.5f, 0.5f);
    cube[14].p = Vector3f(0.0f, -1.0f, 0.0f);

    cube[15] = cube[12];
    cube[15].c = Vector3f(0.0f, -0.5f, -0.5f);
    cube[15].p = Vector3f(0.0f, 1.0f, 0.0f);

    // Left.
    cube[16].c = Vector3f(-0.5f, -0.5f, 0.0f);
    cube[16].u = 0.5f * Vector3f::UnitY();
    cube[16].v = -0.5f * Vector3f::UnitZ();
    cube[16].p = Vector3f(1.0f, 0.0f, 0.0f);
    cube[16].rgba = color;

    cube[17] = cube[16];
    cube[17].c = Vector3f(-0.5f, 0.5f, 0.0f);
    cube[17].p = Vector3f(-1.0f, 0.0f, 0.0f);

    cube[18] = cube[16];
    cube[18].c = Vector3f(-0.5f, 0.0f, 0.5f);
    cube[18].p = Vector3f(0.0f, 1.0f, 0.0f);

    cube[19] = cube[16];
    cube[19].c = Vector3f(-0.5f, 0.0f, -0.5f);
    cube[19].p = Vector3f(0.0f, -1.0f, 0.0f);

    // Right.
    cube[20].c = Vector3f(0.5f, -0.5f, 0.0f);
    cube[20].u = 0.5f * Vector3f::UnitY();
    cube[20].v = 0.5f * Vector3f::UnitZ();
    cube[20].p = Vector3f(1.0f, 0.0f, 0.0f);
    cube[20].rgba = color;

    cube[21] = cube[20];
    cube[21].c = Vector3f(0.5f, 0.5f, 0.0f);
    cube[21].p = Vector3f(-1.0f, 0.0f, 0.0f);

    cube[22] = cube[20];
    cube[22].c = Vector3f(0.5f, 0.0f, 0.5f);
    cube[22].p = Vector3f(0.0f, -1.0f, 0.0f);

    cube[23] = cube[20];
    cube[23].c = Vector3f(0.5f, 0.0f, -0.5f);
    cube[23].p = Vector3f(0.0f, 1.0f, 0.0f);

    m_surfels = std::vector<Surfel>(cube, cube + 24);
}

void
hsv2rgb(float h, float s, float v, float& r, float& g, float& b)
{
    float h_i = std::floor(h / 60.0f);
    float f = h / 60.0f - h_i;

    float p = v * (1.0f - s);
    float q = v * (1.0f - s * f);
    float t = v * (1.0f - s * (1.0f - f));

    switch (static_cast<int>(h_i))
    {
        case 1:
            r = q; g = v; b = p;
            break;
        case 2:
            r = p; g = v; b = t;
            break;
        case 3:
            r = p; g = q; b = v;
            break;
        case 4:
            r = t; g = p; b = v;
            break;
        case 5:
            r = v; g = p; b = q;
            break;
        default:
            r = v; g = t; b = p;
    }
}

void
load_dragon()
{
    m_surfels.resize(m_faces.size());

    for (unsigned int i(0); i < static_cast<unsigned int>(
        m_faces.size()); ++i)
    {
        std::array<unsigned int, 3> face = m_faces[i];
        Vector3f v[3] = {
            m_vertices[face[0]],
            m_vertices[face[1]],
            m_vertices[face[2]]
        };

        Vector3f p0, t1, t2;
#ifndef NDEBUG
		Vector3f &dbg = t2;
		dbg[0] = 0; dbg[1] = 0; dbg[2] = 0;
		std::cout << dbg[0] << " " << dbg[1] << " " << dbg[2] << std::endl;
#endif
        viz->computerPrincipalDirections(
            v[0].data(), v[1].data(), v[2].data(),
            p0.data(), t1.data(), t2.data()
            );

#ifndef NDEBUG
		std::cout << dbg[0] << " " << dbg[1] << " " << dbg[2] << std::endl << std::endl;
#endif

        Vector3f n_s = t1.cross(t2);
        Vector3f n_t = (v[1] - v[0]).cross(v[2] - v[0]);

        if (n_t.dot(n_s) < 0.0f)
        {
            t1.swap(t2);
        }

        m_surfels[i].c = p0;
        m_surfels[i].u = t1;
        m_surfels[i].v = t2;
        m_surfels[i].p = Vector3f::Zero();

        float h = std::min((std::abs(p0.x()) / 0.45f) * 360.0f, 360.0f);
        float r, g, b;
        hsv2rgb(h, 1.0f, 1.0f, r, g, b);
        m_surfels[i].rgba = static_cast<unsigned int>(r * 255.0f)
                         | (static_cast<unsigned int>(g * 255.0f) << 8)
                         | (static_cast<unsigned int>(b * 255.0f) << 16);
    }
}

}

int
main(int argc, char* argv[])
{
    GLviz::init(argc, argv);

    camera.translate(Eigen::Vector3f(0.0f, 0.0f, -2.0f));
    viz = std::unique_ptr<SplatRenderer>(new SplatRenderer(camera));

	///*
    std::string filename = "stanford_dragon_v40k_f80k.raw";
    if (argc > 1)
        filename = argv[1];

    try {
        load_triangle_mesh(filename);
    } catch(std::runtime_error const& e) {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
	//*/

    //load_plane(200);
    //load_cube();
	load_dragon();

    GLviz::display_callback(displayFunc);
    GLviz::reshape_callback(reshapeFunc);
    GLviz::close_callback(closeFunc);

    return GLviz::exec(camera);
}
