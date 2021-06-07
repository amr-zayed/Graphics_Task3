#include "objLoader.h"



Model::Model(char *path) : path(path)
{
    load(path);
}

void Model::load(char *path)
{
    this->path = path;
    model = glmReadOBJ(path);
    glmUnitize(model);             
    glmFacetNormals(model);       
    glmVertexNormals(model, 90.0); 
}

void Model::draw()
{
    //render with vertex normal, texture and materials
    glmDraw(model, GLM_SMOOTH | GLM_MATERIAL ); 
}

void Model::scale(float factor)
{
    glmScale(model, factor);
}

GLMmodel* Model::GetModel()
{
	return model;
}