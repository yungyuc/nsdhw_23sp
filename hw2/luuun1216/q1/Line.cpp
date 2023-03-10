# include "Line.hpp"
# include <iostream>

Line::Line(){}

Line::Line(Line const & line){
    this->vector_x.assign(line.vector_x.begin(), line.vector_x.end());
    this->vector_y.assign(line.vector_y.begin(), line.vector_y.end());
}

Line::Line(Line && line){
    this->vector_x.assign(line.vector_x.begin(), line.vector_x.end());
    this->vector_y.assign(line.vector_y.begin(), line.vector_y.end());
}

Line & Line::operator = (Line const & line){
    vector_x = line.vector_x;
    vector_y = line.vector_y;

    return *this;
}

Line & Line::operator = (Line && line){
    vector_x = line.vector_x;
    vector_y = line.vector_y;

    return *this;
}

Line::Line(size_t size){
    this->vector_x.assign(size,0);
    this->vector_y.assign(size,0);
}

Line::~Line(){
    this->vector_x.clear();
    this->vector_y.clear();
}

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