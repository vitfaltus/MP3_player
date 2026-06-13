//
// Created by archer on 09.06.26.
//


#include "../include/display.hpp"


display::display() {
  this->display = new display(width, height, &Wire, -1);
}
