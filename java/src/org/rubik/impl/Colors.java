package org.rubik.impl;

// Define cube colors
public enum Colors {

     // Regular Colors. Normal color, no bold, background color etc.
     WHITE("\033[1;37m"),
     YELLOW("\033[1;33m"),
     BLUE("\033[1;34m"),
     GREEN("\033[1;32m"),
     RED("\033[1;31m"),
     ORANGE("\u001b[1;38;5;166m");

     //Color end string, color reset
     private static final String RESET = "\033[0m";

     private final String code;

     Colors(String code) {
          this.code = code;
     }

     @Override
     public String toString() {
          String letter = super.toString().substring(0, 1);

          return code + letter + RESET;
     }
}
