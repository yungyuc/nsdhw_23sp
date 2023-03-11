#include <iostream>
#include "Line.hpp"

Line::Line(){}

Line::Line(Line const & my_line){
    x_coordinates = my_line.x_coordinates;
    y_coordinates = my_line.y_coordinates;
}

Line::Line(Line && my_line){
    x_coordinates = my_line.x_coordinates;
    y_coordinates = my_line.y_coordinates;
}

Line & Line::operator = (Line const & my_line){
    x_coordinates = my_line.x_coordinates;
    y_coordinates = my_line.y_coordinates;
    return *this;
}

Line & Line::operator = (Line && my_line){
    x_coordinates = my_line.x_coordinates;
    y_coordinates = my_line.y_coordinates;
    return *this;
}

Line::Line(size_t size){
    this->x_coordinates.resize(size);
    this->y_coordinates.resize(size);
}

Line::~Line(){
    this->x_coordinates.clear();
    this->y_coordinates.clear();
}

size_t Line::size() const{ return this->x_coordinates.size(); }

const float & Line::x(size_t it) const{ return this->x_coordinates.at(it); }

float & Line::x(size_t it){ return this->x_coordinates.at(it); }

const float & Line::y(size_t it) const{ return this->y_coordinates.at(it); }

float & Line::y(size_t it){ return this->y_coordinates.at(it); }