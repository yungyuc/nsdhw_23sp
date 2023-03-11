#include<iostream>
#include<vector>
#include<utility>
#include "line.hpp"



Line::Line() {
    throw std::invalid_argument("Should have specified the size of Line or from other Line");
}

Line::Line(Line const &ano) {
    storage = ano.storage;
}
Line::Line(Line &&ano) {
    storage = ano.storage;
}

Line & Line::operator=(Line const &ano) {
    this->storage = ano.storage;
    return *this;
}

Line & Line::operator=(Line  &&ano) {
    this->storage = ano.storage;
    return *this;
}
Line::Line(size_t size) {
    if(size <= 0) {
        throw std::invalid_argument("Size should greater than 0");
    }
    storage.resize(size);
}
Line::~Line() {
    storage.clear();
}
size_t Line::size() const{
    return this->storage.size();
}

float const & Line::x(size_t it) const {
    if(it >= size()) {
        throw std::length_error("Reference Line out of bound!");
    }
    if(it < 0) {
        throw std::invalid_argument("Reference index should greater than 0!");
    }
    return this->storage[it].first;
}
float & Line::x(size_t it) {
    if(it >= size()) {
        throw std::length_error("Reference Line out of bound!");
    } 
    if(it < 0) {
        throw std::invalid_argument("Reference index should greater than 0!");
    }
    return this->storage[it].first;
}
float const & Line::y(size_t it) const {
    if(it >= size()) {
        throw std::length_error("Reference Line out of bound!");
    }
    if(it < 0) {
        throw std::invalid_argument("Reference index should greater than 0!");
    }
    return this->storage[it].second;
}
float & Line::y(size_t it) {
    
    if(it >= size()) {
        throw std::length_error("Reference Line out of bound!");
    }
    if(it < 0) {
        throw std::invalid_argument("Reference index should greater than 0!");
    }
    return this->storage[it].second;
}
