#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <application.h>
#include <stdio.h>
#include "program.h"
#include "mesh.h"

Mesh::Mesh(std::vector<GLfloat> points, std::vector<GLfloat> textureCoords,
        Texture * texture, Program * program, GLenum drawType, GLenum drawShape) :
    points(points),
    textureCoords(textureCoords),
    texture(texture),
    program(program),
    drawType(drawType),
    drawShape(drawShape)
{
    // Bind program
    program->bind();

    // Make a VBO to hold points
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, points.size() * sizeof(GLfloat), &points[0], drawType);

    // VBO TO hold colors
    GLuint colorVbo = 0;
    glGenBuffers(1, &colorVbo);
    glBindBuffer(GL_ARRAY_BUFFER, colorVbo);

    // Make a VAO for the points VBO
    // TODO: Get indices from shaders
    vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // Color if no texture coordinates
    // TODO: Remove this and find a better way of testing
    if (textureCoords.empty())
    {
        static const float colors[] = {
                1.0, 1.0, 1.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0
        };

        // Assign data to vbo
        glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
        // Describe data in vao
        GLuint colorIndex = program->getAttrib("color");
        glVertexAttribPointer(colorIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(colorIndex);
    }
    else
    {
        // Assign data to vbo
        glBufferData(GL_ARRAY_BUFFER, textureCoords.size() * sizeof(GLfloat), &textureCoords[0], drawType);
        // Describe data in vao
        GLuint texCoordIndex = program->getAttrib("texCoord");
        glVertexAttribPointer(texCoordIndex, 2, GL_FLOAT, GL_FALSE, 0, NULL);
        glEnableVertexAttribArray(texCoordIndex);
    }

    // Points
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    GLuint positionIndex = program->getAttrib("position");
    glVertexAttribPointer(positionIndex, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    glEnableVertexAttribArray(positionIndex);

    // Upload model matrix
    GLint uniModel = program->getUniform("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Unbind program
    program->unbind();
}

Mesh::~Mesh()
{
//TODO: Delete vbos and vaos
}

void Mesh::draw()
{
    // Bind program
    program->bind();
    // Bind vao
    glBindVertexArray(vao);
    // TODO: Bind texture here

    // TODO: Associate with camera instead of here
    glUniformMatrix4fv(program->getUniform("view"), 1, GL_FALSE,
         glm::value_ptr(glm::lookAt(glm::vec3(0.0, 0.0, 0.0),
                 glm::vec3(0.0f, 0.0f, -0.01),
                 glm::vec3(0.0f, 1.0f, 0.0f))));

    // Draw Mesh
    glDrawArrays(drawShape, 0, points.size());

    // TODO: See if unbinding is costly
    // Unbind vao
    glBindVertexArray(0);
    // Unbind program
    program->unbind();
}

glm::vec3 Mesh::move(glm::vec3 translation)
{
    int dims = modelMatrix.length() - 1;
    // Apply translation
    for(int i = 0; i < dims; i++)
        modelMatrix[dims][i] += translation[i];

    // Upload new model matrix
    // TODO: Figure out about binding/unbinding program here
    program->bind();
    GLint uniModel = program->getUniform("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    program->unbind();

    return Entity::move(translation);
}

glm::vec3 Mesh::setPosition(glm::vec3 newPosition)
{
    // Set new position
    int dims = modelMatrix.length() - 1;
    for (int i = 0; i < dims; i++)
        modelMatrix[dims][i] = newPosition[i];

    // Upload new model matrix
    program->bind();
    GLint uniModel = program->getUniform("model");
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(modelMatrix));
    program->unbind();

    return Entity::setPosition(newPosition);
}

glm::mat4 Mesh::getModelMatrix() { return modelMatrix; }