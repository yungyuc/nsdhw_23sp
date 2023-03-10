# include "Line.hpp"
# include <iostream>

Line::Line(){}
Line::Line(size_t size){
    this->vector_x.assign(size,0);
    this->vector_y.assign(size,0);
}

Line::~Line(){}

size_t Line::size() const{
    return this->vector_x.size();
}

const float & Line::x(size_t it) const{
    return this->vector_x.at(it);
}

float & Line::x(size_t it){
    return this->vector_x.at(it);
}

const float & Line::y(size_t it) const{
    return this->vector_y.at(it);
}

float & Line::y(size_t it){
    return this->vector_y.at(it);
}
