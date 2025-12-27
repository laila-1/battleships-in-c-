#include <SFML/Graphics.hpp>
#include <optional>
#include <iostream>
#include <string>
#include <random>
#include <chrono>
#include <thread>


using namespace std;


int main()
{

// lines for randomization- to be used later
random_device rd;
mt19937 gen(rd());


uniform_int_distribution<> pos_dist(0, 9);
uniform_int_distribution<> ship_dist(1, 5);
uniform_int_distribution<> angle_dist(0, 1);

int new_game = 1;


// later --> add the game loop
do{

int user[10][10] = {};
int computer[10][10] = {};

int show_comp[10][10] = {};

bool comp_ships_placed = 0;

int ships_user = 17;
int ships_comp = 17;

bool turn = 0;

bool computersturn = false;  
std::chrono::steady_clock::time_point compturnstart;  

int ship_lockdown = 0;
int lockdown_row, lockdown_col;

// making the actual window with the game, defining its height and width and also the name at the top of the window
sf::RenderWindow window(
    sf::VideoMode(sf::Vector2u(1400, 900)), 
    "Battleships Game- GUI version"
);

sf::Font font;
if (!font.openFromFile("arial.ttf")) {
return -1;

}

using clock_type = chrono::steady_clock;
auto game_start = clock_type::now();

//special colors

sf::Color greyColor(128, 128, 128); // for user ships 
sf::Color lightShipGrey(180, 180, 180); // for unhit user ships
sf::Color lightGrey(220, 220, 220); 
sf::Color unhit(102, 140, 217);  // for unhit areas of comp board and ocean of user
sf::Color oceanBlue(18, 78, 201);  // for misses on either side

// 0--> unhit ocean (unhit)
//1--> unhit ship (lightShipGrey)
// 2--> hit ship (greyColor)
// 3--> hit ocean (oceanBlue)



string name = "";


// for putting text in sfml, you need 3 lines defining the text.


sf::Text introtext(font, "Welcome to Battleships!", 30);
//first line sets the string/variable name, its font, what it says, then its font size

introtext.setPosition(sf::Vector2f(550.f, 50.f));
// this function (.setPosition) sets where the text will be. the first number is the distance from the left side, and the second number is the distance from the top

introtext.setFillColor(sf::Color::Black);
//this function (.setFillColor) sets the color of the variable


sf::Text namereq(font, "Please enter your name:", 24);
namereq.setPosition(sf::Vector2f(50.f, 120.f));
namereq.setFillColor(sf::Color::Black);


sf::Text nameinp(font, "", 20);
nameinp.setPosition(sf::Vector2f(50.f, 165.f));
nameinp.setFillColor(sf::Color::Black);


sf::RectangleShape namebox(sf::Vector2f(200.f, 30.f)); // setting width and height of rectangle (width 200, height 30)
namebox.setPosition(sf::Vector2f(50.f, 165.f));
namebox.setFillColor(lightGrey);


sf::RectangleShape enterbutton(sf::Vector2f(100, 40));
enterbutton.setPosition(sf::Vector2f(300.f, 160.f));
enterbutton.setFillColor(sf::Color::Green);


sf::Text text_on_enterbutton(font, "Enter", 20);
text_on_enterbutton.setPosition(sf::Vector2f(310.f, 165.f)); // not the exact same position as the button itself so that it's more in the center
text_on_enterbutton.setFillColor(sf::Color::White);



sf::RectangleShape easybutton(sf::Vector2f(150.f, 50.f));
easybutton.setPosition(sf::Vector2f(50.f, 180.f));
easybutton.setFillColor(sf::Color::Green);


sf::Text text_on_easybutton(font, "Easy", 24);
text_on_easybutton.setFillColor(sf::Color::White);
text_on_easybutton.setPosition(sf::Vector2f(95.f, 190.f)); 


sf::RectangleShape hardbutton(sf::Vector2f(150.f, 50.f));
hardbutton.setPosition(sf::Vector2f(250.f, 180.f));
hardbutton.setFillColor(sf::Color::Red);


sf::Text text_on_hardbutton(font, "Hard", 24);
text_on_hardbutton.setFillColor(sf::Color::White);
text_on_hardbutton.setPosition(sf::Vector2f(295.f, 190.f)); 


sf::RectangleShape manualbutton(sf::Vector2f(150.f, 50.f));
manualbutton.setPosition(sf::Vector2f(50.f, 100.f));
manualbutton.setFillColor(sf::Color::Blue);


sf::Text text_on_manualbutton(font, "Manual", 24);
text_on_manualbutton.setFillColor(sf::Color::White);
text_on_manualbutton.setPosition(sf::Vector2f(85.f, 110.f));


sf::RectangleShape autobutton(sf::Vector2f(150.f, 50.f));
autobutton.setPosition(sf::Vector2f(250.f, 100.f));
autobutton.setFillColor(sf::Color::Magenta);


sf::Text text_on_autobutton(font, "Automatic", 24);
text_on_autobutton.setFillColor(sf::Color::White);
text_on_autobutton.setPosition(sf::Vector2f(265.f, 110.f));


sf::RectangleShape nextbutton(sf::Vector2f(100.f, 50.f));
nextbutton.setPosition(sf::Vector2f(1250.f, 820.f));
nextbutton.setFillColor(sf::Color::Green);


sf::Text text_on_nextbutton(font, "Next", 24);
text_on_nextbutton.setFillColor(sf::Color::White);
text_on_nextbutton.setPosition(sf::Vector2f(1270.f, 830.f));


sf::Text feedback_text(font, "", 28);
feedback_text.setFillColor(sf::Color::Black);
feedback_text.setPosition(sf::Vector2f(445.f, 150.f));


sf::Text gameover_text(font, "", 36);
gameover_text.setFillColor(sf::Color::Black);
gameover_text.setPosition(sf::Vector2f(300.f, 100.f));



sf::RectangleShape newgamebutton(sf::Vector2f(250.f, 120.f));
newgamebutton.setPosition(sf::Vector2f(300.f, 250.f));
newgamebutton.setFillColor(sf::Color::Green);


sf::Text newgametext(font, "New Game", 32);
newgametext.setFillColor(sf::Color::White);
newgametext.setPosition(sf::Vector2f(340.f, 290.f));



sf::RectangleShape endgamebutton(sf::Vector2f(250.f, 120.f));
endgamebutton.setPosition(sf::Vector2f(800.f, 250.f));
endgamebutton.setFillColor(sf::Color::Red);


sf::Text endgametext(font, "End Game", 32);
endgametext.setFillColor(sf::Color::White);
endgametext.setPosition(sf::Vector2f(840.f, 290.f));



bool hardmode = false; // stores the user choice
bool manual = false;
bool ships_placed = false;

bool carrierdone = false, battleshipdone = false, cruiserdone = false, submarinedone = false, destroyerdone = false;
bool carrierver = false, battleshipver = false, cruiserver = false, submarinever = false, destroyerver = false;


//setting the names of each page that will be visible to the player
enum class page { starting, choosing, placing, thegame, ending };
page state = page::starting;



//defining stuff related to the grid, like the size of each cell, as well as its total height and width (500x500)

const int CELL_SIZE = 50;
const int GRID_SIZE = 10;
const int GRID_WIDTH = CELL_SIZE * GRID_SIZE;
const int GRID_HEIGHT = CELL_SIZE * GRID_SIZE;


// distance from borders to the grid

const int OFFSET_X = (1400 - GRID_WIDTH) / 2;
const int OFFSET_Y = (1000 - GRID_HEIGHT) / 2;



// same thing but for the page when both computer and user boards are there, so distances will be different.

const int OFFSET_USER_X = (800 - GRID_WIDTH) / 2;
const int OFFSET_USER_Y = (1000 - GRID_HEIGHT) / 2;

const int OFFSET_COMP_X = (2000 - GRID_WIDTH) / 2;
const int OFFSET_COMP_Y = (1000 - GRID_HEIGHT) / 2;



// for manual mode, we'll need ships to drag. these will be the ships.

sf::RectangleShape carrierrect(sf::Vector2f(5 * CELL_SIZE, CELL_SIZE));  // width is 500, height is 50
sf::RectangleShape battleshiprect(sf::Vector2f(4 * CELL_SIZE, CELL_SIZE)); // (400x50) and so on
sf::RectangleShape cruiserrect(sf::Vector2f(3 * CELL_SIZE, CELL_SIZE));
sf::RectangleShape submarinerect(sf::Vector2f(3 * CELL_SIZE, CELL_SIZE));
sf::RectangleShape destroyerrect(sf::Vector2f(2 * CELL_SIZE, CELL_SIZE));


carrierrect.setPosition(sf::Vector2f(50.f, 300.f));
battleshiprect.setPosition(sf::Vector2f(50.f, 360.f));
cruiserrect.setPosition(sf::Vector2f(50.f, 420.f));
submarinerect.setPosition(sf::Vector2f(50.f, 480.f));
destroyerrect.setPosition(sf::Vector2f(50.f, 540.f));


carrierrect.setFillColor(greyColor);
battleshiprect.setFillColor(greyColor);
cruiserrect.setFillColor(greyColor);
submarinerect.setFillColor(greyColor);
destroyerrect.setFillColor(greyColor);


bool dragging = false;

// stores the value of the ship currently being dragged (1--> carrier , 2--> battleship , and so on)
int draggedShip = 0;

int grow, gcolumn;


while (window.isOpen())
{

    while (auto event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            // if user presses the x at the top of the window, game ends and window closes
            new_game = 0;
            window.close();
        }

        if (state == page::starting) {
            if (const auto* textEvent = event->getIf<sf::Event::TextEntered>()) {  // this line means if the user enters some sort of text / presses a button
                if (textEvent->unicode == 8) { // backspace
                    if (!name.empty()) name.pop_back(); // if there's a character to be deleted, delete it
                } else if (textEvent->unicode < 128) { // normal chars
                    name += static_cast<char>(textEvent->unicode);  //adds the character of the button pressed to the string
                }
                // giving the nameinp text the value of the name given so that we can display it
                nameinp.setString(name);
            }
        
            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) { // if user left clicks
sf::Vector2f mousePos(mouseEvent->position.x, mouseEvent->position.y); // this is a temporary variable that stores where the click was
if (enterbutton.getGlobalBounds().contains(mousePos)) { // basically: if the user clicked on a place thats within the bounds of the enter button
    if (!name.empty()) { // if theres a name thats entered, go on to the next page
        state = page::choosing;
    }
}

}


        }
        else if (state == page::choosing) {
if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
    sf::Vector2f mousePos(mouseEvent->position.x, mouseEvent->position.y);

    if (easybutton.getGlobalBounds().contains(mousePos)) {
        hardmode = false;
        state = page::placing; 
    }
    else if (hardbutton.getGlobalBounds().contains(mousePos)) {
        hardmode = true;
        state = page::placing; 
    }
}

}

else if (state == page::placing) {

    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mousePos(mouseEvent->position.x, mouseEvent->position.y);

        if (manualbutton.getGlobalBounds().contains(mousePos)) {
            manual = true;
            // resetting the user array for when game is restarted
            for (int i = 0; i < 10; i++) {
               for (int j = 0; j < 10; j++) {
                   user[i][j] = 0;
             }
            }
        }
        else if (autobutton.getGlobalBounds().contains(mousePos) && ships_placed == 0) {
            manual = false;
            ships_placed = false;

            for (int i = 0; i < 10; i++) {
                for (int j = 0; j < 10; j++) {
                    user[i][j] = 0;
                  }
             }

            carrierdone = battleshipdone = cruiserdone = submarinedone = destroyerdone = false;
            carrierver = battleshipver = cruiserver = submarinever = destroyerver = false;
            
                    
// randomizing user board
int carrier2 = 5, battleship2 = 4, cruiser2 = 3, submarine2 = 3, destroyer2 = 2;
int sum2 = carrier2 + battleship2 + cruiser2 + submarine2 + destroyer2;


while (sum2 != 0){
int row_ran= pos_dist(gen);
int column_ran = pos_dist(gen);
int ship_ran = ship_dist(gen);
int angle_ran = angle_dist(gen);


//making sure you can't place the same ship twice
if (ship_ran == 1 && carrier2 == 0)continue;
if (ship_ran == 2 && battleship2 == 0)continue;
if (ship_ran == 3 && cruiser2 == 0)continue;
if (ship_ran == 4 && submarine2 == 0)continue;
if (ship_ran == 5 && destroyer2 == 0)continue;


bool available2 = 0;


int stopper2;
if (ship_ran == 1)stopper2 = 5;
if (ship_ran == 2)stopper2 = 4;
if (ship_ran == 3)stopper2 = 3;
if (ship_ran == 4)stopper2=3;
if (ship_ran == 5)stopper2=2;


// 1 --> H   0 --> V
// checking if ship is able to be placed there
if (angle_ran == 1){
for (int i = column_ran,cntr = 1;cntr <= stopper2;i++, cntr++){
if (i > 9){
available2 = 1; break;
}
if (user[row_ran][i] == 1){
available2 = 1; break;
}
}
}
if (angle_ran == 0){
for (int i = row_ran,cntr = 1;cntr<= stopper2;i++,cntr++){
if (i > 9){
available2 = 1; break;
}
if (user[i][column_ran] == 1){
available2 = 1; break;
}
}
}
if (available2 == 1) continue;
if (available2 == 0) {
if (ship_ran == 1){
sum2-=5; carrier2 = 0;
}
if (ship_ran == 2){
battleship2 = 0;
sum2-=4;}
if (ship_ran == 3){
cruiser2 = 0;
sum2-=3;}
if (ship_ran == 4){
submarine2 = 0;
sum2-=3;}
if (ship_ran == 5){
destroyer2 = 0;
sum2-=2;}

int stopper3;

if (angle_ran == 1){

if (ship_ran == 1)stopper3 = 5;
if (ship_ran == 2)stopper3 = 4;
if (ship_ran == 3)stopper3 = 3;
if (ship_ran == 4)stopper3=3;
if (ship_ran == 5)stopper3=2;


    for (int i = column_ran, cntr = 1;cntr <= stopper3; i++,cntr++){
        user[row_ran][i] = 1;

    }

}


if (angle_ran == 0){

if (ship_ran == 1)stopper3 = 5;
if (ship_ran == 2)stopper3 = 4;
if (ship_ran == 3)stopper3 = 3;
if (ship_ran == 4)stopper3=3;
if (ship_ran == 5)stopper3=2;


    for (int i = row_ran, cntr = 1;cntr <= stopper3; i++,cntr++){
        user[i][column_ran] = 1;
        
    }

}
}
}


ships_placed = true; 
        }

        if (manual) {

    if (carrierrect.getGlobalBounds().contains(mousePos) && !carrierdone) {
        dragging = true;
        draggedShip = 1;

        int length = 5;
        sf::Vector2f size;

        if (carrierver) size = sf::Vector2f(CELL_SIZE, length * CELL_SIZE); // if it's vertical make its width one cell size, and its height 5 cells
        else size = sf::Vector2f(length * CELL_SIZE, CELL_SIZE);

        carrierrect.setSize(size);
    }

    else if (battleshiprect.getGlobalBounds().contains(mousePos) && !battleshipdone) {
        dragging = true;
        draggedShip = 2;

        int length = 4;
        sf::Vector2f size;

        if (battleshipver) size = sf::Vector2f(CELL_SIZE, length * CELL_SIZE);
        else size = sf::Vector2f(length * CELL_SIZE, CELL_SIZE);

        battleshiprect.setSize(size);
    }

    else if (cruiserrect.getGlobalBounds().contains(mousePos) && !cruiserdone) {
        dragging = true;
        draggedShip = 3;

        int length = 3;
        sf::Vector2f size;

        if (cruiserver) size = sf::Vector2f(CELL_SIZE, length * CELL_SIZE);
        else size = sf::Vector2f(length * CELL_SIZE, CELL_SIZE);

        cruiserrect.setSize(size);
    }

    else if (submarinerect.getGlobalBounds().contains(mousePos) && !submarinedone) {
        dragging = true;
        draggedShip = 4;

        int length = 3;
        sf::Vector2f size;

        if (submarinever) size = sf::Vector2f(CELL_SIZE, length * CELL_SIZE);
        else size = sf::Vector2f(length * CELL_SIZE, CELL_SIZE);

        submarinerect.setSize(size);
    }

    else if (destroyerrect.getGlobalBounds().contains(mousePos) && !destroyerdone) {
        dragging = true;
        draggedShip = 5;

        int length = 2;
        sf::Vector2f size;

        if (destroyerver) size = sf::Vector2f(CELL_SIZE, length * CELL_SIZE);
        else size = sf::Vector2f(length * CELL_SIZE, CELL_SIZE);

        destroyerrect.setSize(size);
    }
}

    }

    // if R is pressed --> rotate the ship
    if (const auto* keyEvent = event->getIf<sf::Event::KeyPressed>()) {
    if (keyEvent->code == sf::Keyboard::Key::R && dragging && manual) {

        if (draggedShip == 1) {
            carrierver = !carrierver;  //if it was vertical make it horizontal, and vice versa

            int length = 5;
            sf::Vector2f size;

            if (carrierver) size = sf::Vector2f(CELL_SIZE, length * CELL_SIZE);
            else size = sf::Vector2f(length * CELL_SIZE, CELL_SIZE);

            carrierrect.setSize(size);
        }

        else if (draggedShip == 2) {
            battleshipver = !battleshipver;

            int length = 4;
            sf::Vector2f size;

            if (battleshipver) size = sf::Vector2f(CELL_SIZE, length * CELL_SIZE);
            else size = sf::Vector2f(length * CELL_SIZE, CELL_SIZE);

            battleshiprect.setSize(size);
        }

        else if (draggedShip == 3) {
            cruiserver = !cruiserver;

            int length = 3;
            sf::Vector2f size;

            if (cruiserver) size = sf::Vector2f(CELL_SIZE, length * CELL_SIZE);
            else size = sf::Vector2f(length * CELL_SIZE, CELL_SIZE);

            cruiserrect.setSize(size);
        }

        else if (draggedShip == 4) {
            submarinever = !submarinever;

            int length = 3;
            sf::Vector2f size;

            if (submarinever) size = sf::Vector2f(CELL_SIZE, length * CELL_SIZE);
            else size = sf::Vector2f(length * CELL_SIZE, CELL_SIZE);

            submarinerect.setSize(size);
        }

        else if (draggedShip == 5) {
            destroyerver = !destroyerver;

            int length = 2;
            sf::Vector2f size;

            if (destroyerver) size = sf::Vector2f(CELL_SIZE, length * CELL_SIZE);
            else size = sf::Vector2f(length * CELL_SIZE, CELL_SIZE);

            destroyerrect.setSize(size);
        }
    }
}

    if (const auto* moveEvent = event->getIf<sf::Event::MouseMoved>()) {
        if (dragging && manual) {
            sf::Vector2f mousePos(moveEvent->position.x, moveEvent->position.y);
            if (draggedShip == 1) carrierrect.setPosition(mousePos);  // makes the ship follow the mouse (as long as left click is being held)
            if (draggedShip == 2) battleshiprect.setPosition(mousePos);
            if (draggedShip == 3) cruiserrect.setPosition(mousePos);
            if (draggedShip == 4) submarinerect.setPosition(mousePos);
            if (draggedShip == 5) destroyerrect.setPosition(mousePos);
        }
    }

    if (const auto* releaseEvent = event->getIf<sf::Event::MouseButtonReleased>()) {
        if (dragging && manual) {
            dragging = false;

            sf::Vector2f pos;
            int stopper;
            bool vertical = false; // for looping to check if ship is able to be placed there. 1--> vertical , 0 --> horizontal
            if (draggedShip == 1) { pos = carrierrect.getPosition(); stopper = 5; vertical = carrierver; }
            if (draggedShip == 2) { pos = battleshiprect.getPosition(); stopper = 4; vertical = battleshipver; }
            if (draggedShip == 3) { pos = cruiserrect.getPosition(); stopper = 3; vertical = cruiserver; }
            if (draggedShip == 4) { pos = submarinerect.getPosition(); stopper = 3; vertical = submarinever; }
            if (draggedShip == 5) { pos = destroyerrect.getPosition(); stopper = 2; vertical = destroyerver; }

            int column = (pos.x - OFFSET_X) / CELL_SIZE;  // pos.x - offset_x gets where from the start of the GRID the mouse is, and by dividing that by cell size u get in which cell the mouse is. (only works bc it's int, not float)
            int row = (pos.y - OFFSET_Y) / CELL_SIZE;

            bool taken = false;

            if (vertical){
               for (int i = row,cntr = 1;cntr<= stopper;i++,cntr++){
                   if (i > 9){
                       taken = 1; break;
                    }
                 if (user[i][column] == 1){
                    taken = 1; break;
                 }
              }
            }

            else if (!vertical){
                for (int i = column,cntr = 1;cntr <= stopper;i++, cntr++){
                  if (i > 9){
                     taken = 1; break;
                }
                 if (user[row][i] == 1){
                     taken = 1; break;
                 }
             }
            }


            if (!taken) {
                // placing ships


if (!vertical) {   
    for (int i = 0; i < stopper; i++) {
        user[row][column + i] = 1;
    }
}

if (vertical) {    
    for (int i = 0; i < stopper; i++) {
        user[row + i][column] = 1;
    }
}

                sf::Vector2f snapPos(OFFSET_X + column * CELL_SIZE, OFFSET_Y + row * CELL_SIZE); //roughly where the mouse was
                if (draggedShip == 1) { carrierrect.setPosition(snapPos); carrierdone = true; }
                if (draggedShip == 2) { battleshiprect.setPosition(snapPos); battleshipdone = true; }
                if (draggedShip == 3) { cruiserrect.setPosition(snapPos); cruiserdone = true; }
                if (draggedShip == 4) { submarinerect.setPosition(snapPos); submarinedone = true; }
                if (draggedShip == 5) { destroyerrect.setPosition(snapPos); destroyerdone = true; }
            } 
        }
    }
if ( carrierdone == 1 && battleshipdone == 1 && cruiserdone == 1 && submarinedone == 1 && destroyerdone == 1) ships_placed = true;

    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
        sf::Vector2f mousePos(mouseEvent->position.x, mouseEvent->position.y);
        if (nextbutton.getGlobalBounds().contains(mousePos)) {
            if (ships_placed) {
                state = page::thegame;
                feedback_text.setFillColor(sf::Color::Black);
                feedback_text.setString("Your turn! Click on the computer's board.");
            }
        }
    }


if (comp_ships_placed == 0){
        int carrier1 = 5, battleship1 = 4, cruiser1 = 3, submarine1 = 3, destroyer1 = 2;
int sum1 = carrier1 + battleship1 + cruiser1 + submarine1 + destroyer1; 


 while (sum1 > 0){

   int row_comp= pos_dist(gen);
   int column_comp = pos_dist(gen);
   int ship_comp = ship_dist(gen);
   int angle_comp = angle_dist(gen);

if (ship_comp == 1 && carrier1 == 0)continue;
if (ship_comp == 2 && battleship1 == 0)continue;
if (ship_comp == 3 && cruiser1 == 0)continue;
if (ship_comp == 4 && submarine1 == 0)continue;
if (ship_comp == 5 && destroyer1 == 0)continue;

   bool available = 0;

   int stopper;
if (ship_comp == 1)stopper = 5;
    if (ship_comp == 2)stopper = 4;
    if (ship_comp == 3)stopper = 3;
    if (ship_comp == 4)stopper=3;
    if (ship_comp == 5)stopper=2;

// 1 --> H   0 --> V
// checking if ship is able to be placed there
if (angle_comp == 1){
    for (int i = column_comp,cntr = 1;cntr <= stopper;i++, cntr++){
        if (i > 9){
            available = 1; break;
        }
    if (computer[row_comp][i] == 1){
        available = 1; break;
    }
    }
}
if (angle_comp == 0){
    for (int i = row_comp,cntr = 1;cntr<= stopper;i++,cntr++){
        if (i > 9){
            available = 1; break;
        }
    if (computer[i][column_comp] == 1){
        available = 1; break;
    }
    }
}
if (available == 1) continue;
if (available == 0) {

    if (ship_comp == 1){
            sum1-=5; carrier1 = 0;
    }
    if (ship_comp == 2){
            battleship1 = 0;
            sum1-=4;}
    if (ship_comp == 4){
            submarine1 = 0;
            sum1-=3;}
    if (ship_comp == 3){
            cruiser1 = 0;
            sum1-=3;}
    if (ship_comp == 5){
            destroyer1 = 0;
            sum1-=2;}

//making the ships fill the board
int stopper;
    if (angle_comp == 1){

if (ship_comp == 1)stopper = 5;
    if (ship_comp == 2)stopper = 4;
    if (ship_comp == 3)stopper = 3;
    if (ship_comp == 4)stopper=3;
    if (ship_comp == 5)stopper=2;


        for (int i = column_comp, cntr = 1;cntr <= stopper; i++,cntr++){
            computer[row_comp][i] = 1;
        }
}

    if (angle_comp == 0){


if (ship_comp == 1)stopper = 5;
    if (ship_comp == 2)stopper = 4;
    if (ship_comp == 3)stopper = 3;
    if (ship_comp == 4)stopper=3;
    if (ship_comp == 5)stopper=2;


        for (int i = row_comp, cntr = 1;cntr <= stopper; i++,cntr++){
            computer[i][column_comp] = 1;
        }
}
 }

  }


comp_ships_placed = 1;
    }

}


// 0--> unhit ocean (unhit)
//1--> unhit ship (lightShipGrey)
// 2--> hit ship (greyColor)
// 3--> hit ocean (oceanBlue)


else if (state == page::thegame) {

if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
        if (turn == 0) {  
            sf::Vector2f mousePos(mouseEvent->position.x, mouseEvent->position.y);
            if (mousePos.x >= OFFSET_COMP_X && mousePos.x < OFFSET_COMP_X + GRID_WIDTH && mousePos.y >= OFFSET_COMP_Y && mousePos.y < OFFSET_COMP_Y + GRID_HEIGHT) // a lot of math but basically just checking if the mouse is on the computer's board
             {
                int col = (mousePos.x - OFFSET_COMP_X) / CELL_SIZE;
                int row = (mousePos.y - OFFSET_COMP_Y) / CELL_SIZE;
                // making sure it's a hitable block
                if (row >= 0 && row < 10 && col >= 0 && col < 10 && show_comp[row][col] == 0) {
                    if (computer[row][col] == 1) {
                        show_comp[row][col] = 2; 
                        ships_comp--;
                        feedback_text.setFillColor(sf::Color::Green);
                        feedback_text.setString("                     You hit a ship!"); // a lot of spaces just to center the text
                    } else {
                        show_comp[row][col] = 3;  
                        feedback_text.setFillColor(sf::Color::Red);
                        feedback_text.setString("                      You missed.");
                    }
                    turn = 1;  
                    computersturn = true;
                    compturnstart = std::chrono::steady_clock::now(); 
                }

            }
        }
    }


}


else if (state == page::ending) {
    if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
    sf::Vector2f mousePos(mouseEvent->position.x, mouseEvent->position.y);

    if (newgamebutton.getGlobalBounds().contains(mousePos)) {
       
        state = page::starting; // move to the starting page where it asks for ur name again
        for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            user[i][j] = 0;
            computer[i][j] = 0;
            show_comp[i][j] = 0;
        }
    }
    // Reset all variables
    comp_ships_placed = 0;
    ships_user = 17;
    ships_comp = 17;
    turn = 0;
    computersturn = false;
    ship_lockdown = 0;
    hardmode = false;
    manual = false;
    ships_placed = false;
    carrierdone = battleshipdone = cruiserdone = submarinedone = destroyerdone = false;
    carrierver = battleshipver = cruiserver = submarinever = destroyerver = false;
    feedback_text.setString("");
    nameinp.setString("");
    string name = "";

    carrierrect.setPosition(sf::Vector2f(50.f, 300.f));
    carrierrect.setSize(sf::Vector2f(5 * CELL_SIZE, CELL_SIZE)); 
    battleshiprect.setPosition(sf::Vector2f(50.f, 360.f));
    battleshiprect.setSize(sf::Vector2f(4 * CELL_SIZE, CELL_SIZE));
    cruiserrect.setPosition(sf::Vector2f(50.f, 420.f));
    cruiserrect.setSize(sf::Vector2f(3 * CELL_SIZE, CELL_SIZE));
    submarinerect.setPosition(sf::Vector2f(50.f, 480.f));
    submarinerect.setSize(sf::Vector2f(3 * CELL_SIZE, CELL_SIZE));
    destroyerrect.setPosition(sf::Vector2f(50.f, 540.f));
    destroyerrect.setSize(sf::Vector2f(2 * CELL_SIZE, CELL_SIZE));

    game_start = clock_type::now();

    }
    else if (endgamebutton.getGlobalBounds().contains(mousePos)) {
        new_game = 0;
        window.close();
    }
}
}

}

// computer's turn. this was done alone bc of too many errors. the window kept crashing and without the computersturn check the game would crash.
// if done outside the while window is open loop then there wouldn't be any computer ships when the game restarts.


if (state == page::thegame && turn  == 1){

if (state == page::thegame && computersturn) {
    auto elapsed = std::chrono::steady_clock::now() - compturnstart; // measures how much time has passed since computer's turn began
    if (elapsed >= std::chrono::seconds(2)) { // makes sure computer waits 2 seconds before playing its turn 
        while(true){
if (hardmode == 1 && ship_lockdown == 1){
    if (lockdown_row + 1 < 10 && user[lockdown_row + 1][lockdown_col] != 2 && user[lockdown_row + 1][lockdown_col] != 3 ){
        grow = lockdown_row + 1;
        gcolumn = lockdown_col;
    }
    else if (lockdown_row - 1 >= 0 && user[lockdown_row - 1][lockdown_col] != 2 && user[lockdown_row - 1][lockdown_col] != 3){
        grow = lockdown_row - 1;
        gcolumn = lockdown_col;
    }
    else if (lockdown_col + 1 < 10 && user[lockdown_row][lockdown_col + 1] != 2 && user[lockdown_row][lockdown_col + 1] != 3){
        grow = lockdown_row ;
        gcolumn = lockdown_col +1;
    }
    else if (lockdown_col - 1 >= 0 && user[lockdown_row ][lockdown_col - 1] != 2 && user[lockdown_row ][lockdown_col - 1] != 3){
        grow = lockdown_row ;
        gcolumn = lockdown_col - 1;
    }
    else {
        ship_lockdown = 0;
        grow = pos_dist(gen);
        gcolumn = pos_dist(gen);
    }
}
else {grow = pos_dist(gen);
gcolumn = pos_dist(gen); }
if (user[grow][gcolumn] != 2 && user[grow][gcolumn] != 3) break;
}

if (user[grow][gcolumn] == 1){
    feedback_text.setFillColor(sf::Color::Red);
    feedback_text.setString("               Computer has hit a ship.");
    ships_user--;
    user[grow][gcolumn] = 2;
    ship_lockdown = 1;
    lockdown_row = grow; lockdown_col = gcolumn;}
else if (user[grow][gcolumn] == 0){
    feedback_text.setFillColor(sf::Color::Green);
    feedback_text.setString("                 Computer missed.");
    user[grow][gcolumn] = 3;

}
        turn = 0;
        computersturn = false;
    }
}
}


if (state == page::thegame && (ships_user == 0 || ships_comp == 0)) {
    auto game_end = clock_type::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(game_end - game_start);
    if (ships_user == 0) {
        gameover_text.setString("Computer has won. Game duration: " + std::to_string(duration.count()) + " seconds.");
    } else {
        gameover_text.setString(name +" has won! Game duration: " + std::to_string(duration.count()) + " seconds.");
    }
    state = page::ending;
}

window.clear(sf::Color::White);
if (state == page::starting) {

window.draw(introtext);
window.draw(namereq);
window.draw(namebox);
window.draw(nameinp);
window.draw(enterbutton);
window.draw(text_on_enterbutton);

} else if (state == page::choosing) {

sf::Text welcomeText(font, "Welcome " + name + "!", 24);
welcomeText.setFillColor(sf::Color::Black);
welcomeText.setPosition(sf::Vector2f(50.f, 50.f));
window.draw(welcomeText);


sf::Text modeText(font, "Please choose the game mode: Easy or Hard", 24);
modeText.setFillColor(sf::Color::Black);
modeText.setPosition(sf::Vector2f(50.f, 120.f));
window.draw(modeText);


window.draw(easybutton);
window.draw(text_on_easybutton);
window.draw(hardbutton);
window.draw(text_on_hardbutton);


}


else if (state == page::placing){


sf::Text optionText(font, "Please choose whether you'd like to manually place your ships or whether you'd like them to be automatically placed.", 24);
optionText.setFillColor(sf::Color::Black);
optionText.setPosition(sf::Vector2f(50.f, 40.f));
window.draw(optionText);


window.draw(manualbutton);
window.draw(text_on_manualbutton);
window.draw(autobutton);
window.draw(text_on_autobutton);


for (int row = 0; row < GRID_SIZE; ++row) {
for (int col = 0; col < GRID_SIZE; ++col) {
sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
cell.setPosition(sf::Vector2f(OFFSET_X + col * CELL_SIZE, OFFSET_Y + row * CELL_SIZE));


        if (user[row][col] == 1) cell.setFillColor(greyColor); 
        else cell.setFillColor(oceanBlue);

        cell.setOutlineColor(sf::Color::Black);
        cell.setOutlineThickness(2.f);
        window.draw(cell);
    }
}

if (manual) {
// if a ship isn't placed, draw the rectangle that represents it
    if (!carrierdone) window.draw(carrierrect);
    if (!battleshipdone) window.draw(battleshiprect);
    if (!cruiserdone) window.draw(cruiserrect);
    if (!submarinedone)window.draw(submarinerect);
    if (!destroyerdone) window.draw(destroyerrect);
}


if (ships_placed || manual) {


sf::Text gridText(font, "Your randomized board:", 26);
gridText.setFillColor(sf::Color::Black);
gridText.setPosition(sf::Vector2f(85.f, 200.f));
if(manual == 0)
window.draw(gridText);


}




window.draw(nextbutton);
window.draw(text_on_nextbutton);


}

else if(state == page::thegame){

bool exptextshow = 0;


sf::Text explainText(font, "       Both players will take turns hitting each others' boards.\nWhoever successfully destroys the other player's ships first wins.", 34);
explainText.setFillColor(sf::Color::Black);
explainText.setPosition(sf::Vector2f(200.f, 30.f));
if(!exptextshow)window.draw(explainText);
exptextshow = 1; 


sf::Text userboardText(font, name + "'s current board: ", 28);
userboardText.setFillColor(sf::Color::Black);
userboardText.setPosition(sf::Vector2f(145.f, 200.f));
window.draw(userboardText);


    for (int row = 0; row < GRID_SIZE; ++row) {
for (int col = 0; col < GRID_SIZE; ++col) {
sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
cell.setPosition(sf::Vector2f(OFFSET_USER_X + col * CELL_SIZE, OFFSET_USER_Y + row * CELL_SIZE));


        if (user[row][col] == 1) cell.setFillColor(lightShipGrey); // ships
        else if (user[row][col] == 2) cell.setFillColor(greyColor);
        else if (user[row][col] == 3) cell.setFillColor(oceanBlue);
        else cell.setFillColor(unhit);

        cell.setOutlineColor(sf::Color::Black);
        cell.setOutlineThickness(2.f);
        window.draw(cell);
    }
}


sf::Text compboardText(font, "Computer's current board: ", 28);
compboardText.setFillColor(sf::Color::Black);
compboardText.setPosition(sf::Vector2f(750.f, 200.f));
window.draw(compboardText);


for (int row = 0; row < GRID_SIZE; ++row) {
for (int col = 0; col < GRID_SIZE; ++col) {
sf::RectangleShape cell(sf::Vector2f(CELL_SIZE - 2.f, CELL_SIZE - 2.f));
cell.setPosition(sf::Vector2f(OFFSET_COMP_X + col * CELL_SIZE, OFFSET_COMP_Y + row * CELL_SIZE));


        if (show_comp[row][col] == 2){ cell.setFillColor(greyColor); }
        else if (show_comp[row][col] == 3) cell.setFillColor(oceanBlue); 
        else cell.setFillColor(unhit);  // whether there's actually a ship or not, make it the same color since we aren't supposed to see where the computer's ships are

        cell.setOutlineColor(sf::Color::Black);
        cell.setOutlineThickness(2.f);
        window.draw(cell);
    }
}



window.draw(feedback_text);
if (ships_user == 0 || ships_comp == 0) {
    window.draw(gameover_text);
}

}

else if (state == page::ending) {

window.clear(sf::Color::White); // making a white page
window.draw(gameover_text);

window.draw(newgamebutton);
window.draw(newgametext);
window.draw(endgamebutton);
window.draw(endgametext);
    
}

window.display();

}
}
 while(new_game);

return 0;

}