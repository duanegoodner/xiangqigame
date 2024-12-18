#include <iostream>
#include <string>

// ANSI color codes
const std::string RED_TEXT_WHITE_BG = "\033[1;37;41m"; // Bright white text, red background
const std::string BLACK_TEXT_WHITE_BG = "\033[1;30;47m"; // Bright black text, white background
const std::string RESET_FORMAT = "\033[0m"; // Reset formatting

void displayPiece(char piece, const std::string& format) {
    std::cout << format << piece << RESET_FORMAT;
}

int main() {
    std::cout << "Board Example:\n";
    displayPiece('R', RED_TEXT_WHITE_BG); 
    std::cout << " ";
    displayPiece('B', BLACK_TEXT_WHITE_BG);
    std::cout << " ";
    displayPiece('-', RESET_FORMAT);
    std::cout << std::endl;
    

    return 0;
}
