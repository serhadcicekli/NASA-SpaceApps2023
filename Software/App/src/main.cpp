#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <string>
#include <iostream>
#include <vector>

#define APP_DEBUG

#define PHASE_NONE -1
#define PHASE_INTRO 0
#define PHASE_MAIN_MENU 1
#define PHASE_WARNING_SYSTEM_INFO 2
#define PHASE_WARNING_SYSTEM_DEMO 3
#define PHASE_WARNING_L1 4
#define PHASE_WARNING_L2 5
#define PHASE_WARNING_L3 6
#define PHASE_GAME_MENU 7
#define PHASE_QUIZ_GAME_MENU 8
#define PHASE_QUIZ_GAME_START 9
#define PHASE_QUIZ_GAME 10
#define PHASE_QUIZ_GAME_SUMMARY 11
#define A 0
#define B 1
#define C 2
#define D 3
#define min(A, B) (A<B?A:B)
#define max(A, B) (A>B?A:B)
#define loadq(A) editQuiestion(questions[A], sectionA[A], sectionB[A], sectionC[A], sectionD[A], correctAnswer[A]);


#ifdef APP_DEBUG
#define AA_LEVEL 1
#else
#define AA_LEVEL 4
#endif
sf::RenderWindow window;
sf::Vector2i mousePosition;
sf::SoundBuffer l2sb, l3sb, cdb, cdeb, falsesndb, truesndb, summarysndb;
sf::Sound l2s, l3s, cd, cde, falsesnd, truesnd, summarysnd;
bool mouseDown;
const std::string appTitle = "SkyLab21 Project Prototype Application";
const std::string appVersion = "1.0 (DEV)";
sf::Font font;
int wndWidth = 1500, wndHeight = 1000;
class UIButton{
public:
    bool oldmouseState = 0;
    bool buttonDown = 0;
    sf::Text text;
    sf::RectangleShape rectangle;
    UIButton(std::string textstr){
        text.setFont(font);
        setString(textstr);
        text.setFillColor(sf::Color::Black);
        rectangle.setFillColor(sf::Color::White);
    }

    void setPosition(int x, int y){
        rectangle.setPosition(x, y);
        text.setPosition(x + 10, y);
    }

    void setString(std::string txt){
        text.setString(txt);
        rectangle.setSize(sf::Vector2f(text.getGlobalBounds().width + 20, text.getGlobalBounds().height + 20));
    }

    void draw(){
        if(buttonDown){
            rectangle.setFillColor(sf::Color::Yellow);
        }else{
            rectangle.setFillColor(sf::Color::White);
        }
        window.draw(rectangle);
        window.draw(text);
    }

    int getWidth(){
        return rectangle.getGlobalBounds().width;
    }

    int getHeight(){
        return rectangle.getGlobalBounds().height;
    }

    int getX(){
        return rectangle.getGlobalBounds().getPosition().x;
    }

    int getY(){
        return rectangle.getGlobalBounds().getPosition().y;
    }

    int getCenterX(){
        return (wndWidth - getWidth()) / 2;
    }

    int getCenterY(){
        return (wndHeight - getHeight()) / 2;
    }

    int getRight(){
        return getX() + getWidth();
    }

    int getBottom(){
        return getY() + getHeight();
    }

    bool clicked(){
        if(oldmouseState != mouseDown){
            oldmouseState = mouseDown;
            if(mouseDown){
                if(mousePosition.x >= getX() && mousePosition.y >= getY() && getX() + getWidth() >= mousePosition.x && getY() + getHeight() >= mousePosition.y){
                    buttonDown = 1;
                }
            }else{
                if(buttonDown){
                    buttonDown = 0;
                    if(mousePosition.x >= getX() && mousePosition.y >= getY() && getX() + getWidth() >= mousePosition.x && getY() + getHeight() >= mousePosition.y){
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
};
unsigned int currentFrame = 0;
class UILabel{
public:
    bool oldmouseState = 0;
    bool buttonDown = 0;
    sf::Text text;
    UILabel(std::string textstr){
        text.setFont(font);
        setString(textstr);
        text.setFillColor(sf::Color::White);
    }

    void setPosition(int x, int y){
        text.setPosition(x, y);
    }

    void setString(std::string txt){
        text.setString(txt);
    }

    void draw(){
        window.draw(text);
    }

    int getWidth(){
        return text.getGlobalBounds().width;
    }

    int getHeight(){
        return text.getGlobalBounds().height;
    }

    int getX(){
        return text.getGlobalBounds().getPosition().x;
    }

    int getY(){
        return text.getGlobalBounds().getPosition().y;
    }

    int getCenterX(){
        return (wndWidth - getWidth()) / 2;
    }

    int getCenterY(){
        return (wndHeight - getHeight()) / 2;
    }

    int getRight(){
        return getX() + getWidth();
    }

    int getBottom(){
        return getY() + getHeight();
    }

    bool clicked(){
        if(oldmouseState != mouseDown){
            oldmouseState = mouseDown;
            if(mouseDown){
                if(mousePosition.x >= getX() && mousePosition.y >= getY() && getX() + getWidth() >= mousePosition.x && getY() + getHeight() >= mousePosition.y){
                    buttonDown = 1;
                }
            }else{
                if(buttonDown){
                    buttonDown = 0;
                    if(mousePosition.x >= getX() && mousePosition.y >= getY() && getX() + getWidth() >= mousePosition.x && getY() + getHeight() >= mousePosition.y){
                        return 1;
                    }
                }
            }
        }
        return 0;
    }
};
UILabel quizText("");
UIButton quizA(""), quizB(""), quizC(""), quizD("");
int quizCorrectAnswer;
void editQuiestion(std::string question, std::string a, std::string b, std::string c, std::string d, int correct){
    quizText.setString(question);
    quizA.setString("A: " + a);
    quizB.setString("B: " + b);
    quizC.setString("C: " + c);
    quizD.setString("D: " + d);
    quizText.setPosition(quizText.getCenterX(), 100);
    quizA.setPosition(quizA.getCenterX(), quizText.getBottom() + 30);
    quizB.setPosition(quizB.getCenterX(), quizA.getBottom() + 30);
    quizC.setPosition(quizC.getCenterX(), quizB.getBottom() + 30);
    quizD.setPosition(quizD.getCenterX(), quizC.getBottom() + 30);
    quizCorrectAnswer = correct;
}
int main(){
    int phase = PHASE_INTRO;
    sf::ContextSettings settings;
    settings.antialiasingLevel = AA_LEVEL;
    window.create(sf::VideoMode(wndWidth, wndHeight), appTitle + " v" + appVersion, sf::Style::Titlebar | sf::Style::Close, settings);
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);
    if(!font.loadFromFile("font.ttf")){
        std::cout << "Failed to load font!" << std::endl;
        return EXIT_FAILURE;
    }
    sf::Text logoTitle;
    logoTitle.setFont(font);
    logoTitle.setString("Application");
    logoTitle.setPosition((wndWidth - logoTitle.getGlobalBounds().width) / 2, (wndHeight + 200) / 2);
    

    sf::Texture logoTexture;
    logoTexture.loadFromFile("skylab21.png");
    sf::Sprite logoSprite;
    logoSprite.setTexture(logoTexture, true);
    logoSprite.setPosition((wndWidth - logoSprite.getGlobalBounds().width) / 2, (wndHeight - logoSprite.getGlobalBounds().height) / 2 - 60);
    unsigned long frameCounter = 0;
    sf::Texture bgTx;
    bgTx.loadFromFile("bg.jpg");
    sf::Sprite bgSprite(bgTx);
    l2sb.loadFromFile("level2notification.wav");
    l3sb.loadFromFile("level3notification.wav");
    cdb.loadFromFile("countdown.wav");
    cdeb.loadFromFile("countdown-end.wav");
    cd.setBuffer(cdb);
    cde.setBuffer(cdeb);
    l2s.setBuffer(l2sb);
    l3s.setBuffer(l3sb);
    UIButton backButton("Back");
    backButton.setPosition(wndWidth - backButton.getWidth() - 30, wndHeight - backButton.getHeight() - 30);

    //Main Menu
    UILabel titleLabel("Skylab21 Project Prototype Application");
    titleLabel.setPosition(titleLabel.getCenterX(), 100);
    UIButton warningSystemButton("About warning system");
    warningSystemButton.setPosition(warningSystemButton.getCenterX(), warningSystemButton.getCenterY() - 100);
    UIButton gameMenuButton("Game");
    gameMenuButton.setPosition(gameMenuButton.getCenterX(), warningSystemButton.getBottom() + 20);
    UIButton exitButton("Exit");
    exitButton.setPosition(exitButton.getCenterX(), gameMenuButton.getBottom() + 20);
    
    //Warning system info
    UILabel warningSystemTitle("Warning system information");
    warningSystemTitle.setPosition(30, 30);
    UILabel aboutWarningSystem("This warning system aims to notify people of possible dust storms\n with the information measured by the EMIT instrument.");
    aboutWarningSystem.setPosition(aboutWarningSystem.getCenterX(), aboutWarningSystem.getCenterY() - 100);
    UIButton warningSystemDemoButton("Test warning system");
    warningSystemDemoButton.setPosition(warningSystemDemoButton.getCenterX(), aboutWarningSystem.getBottom() + 30);
    
    //Warning system demo
    UILabel warningSystemDemoTitle("Warning system demo");
    warningSystemDemoTitle.setPosition(30, 30);
    UILabel warningSystemDemoDescription("Select one of the following levels to test how storm warnings will operate.\n(Note: The alert you select will be activated in 3 seconds)");
    warningSystemDemoDescription.setPosition(warningSystemDemoDescription.getCenterX(), warningSystemDemoDescription.getCenterY() - 150);
    UIButton wsdl1("Level 1 (3 or more days to storm)");
    wsdl1.setPosition(wsdl1.getCenterX(), warningSystemDemoDescription.getBottom() + 30);
    UIButton wsdl2("Level 2 (24 hours to storm)");
    wsdl2.setPosition(wsdl2.getCenterX(), wsdl1.getBottom() + 30);
    UIButton wsdl3("Level 3 (1 hour or less to the storm)");
    wsdl3.setPosition(wsdl3.getCenterX(), wsdl2.getBottom() + 30);

    UILabel wltitle("Dust storm warning");
    wltitle.setPosition(30, 30);
    //Warning level 1
    UILabel wl1Msg("Potential dust storm expected over 3 days. Regions to be affected:\n Diyarbakir, Sanliurfa, Gaziantep");
    wl1Msg.setPosition(wl1Msg.getCenterX(), wl1Msg.getCenterY());
    //Warning level 2
    UILabel wl2Msg("Potential dust storm is expected in less than 24 hours. Regions to be affected:\n Diyarbakir, Sanliurfa, Gaziantep");
    wl2Msg.setPosition(wl2Msg.getCenterX(), wl2Msg.getCenterY());
    //Warning level 3
    UILabel wl3Msg("ATTENTION!\n A dust storm is expected in less than 1 hour. \nRegions to be affected:\n Diyarbakir, Sanliurfa, Gaziantep. \n\nPLEASE BE CAUTIOUS!");
    wl3Msg.setPosition(wl3Msg.getCenterX(), wl3Msg.getCenterY());
    int selectedWarning = 0;
    
    //Game Menu
    UILabel gameMenuTitle("Game Menu");
    gameMenuTitle.setPosition(30, 30);
    UILabel aboutGame("The aim of these games is to strengthen users' understanding of\nwhat EMIT is all about and to make it a little bit fun.\nYou can experience the games shown below.");
    aboutGame.setPosition(aboutGame.getCenterX(), aboutGame.getCenterY() - 100);
    UIButton quizGameButton("Play quiz game");
    quizGameButton.setPosition(quizGameButton.getCenterX(), aboutGame.getBottom() + 30);

    //Quiz game menu
    UILabel quizMenuTitle("Quiz Game");
    quizMenuTitle.setPosition(30, 30);
    UILabel quizMenuAbout("The quiz game will strengthen your knowledge about EMIT.\nIn addition, each correct answer within a certain time period earns you points.\nYou can use these points in other games.");
    quizMenuAbout.setPosition(quizMenuAbout.getCenterX(), quizMenuAbout.getCenterY() - 60);
    UIButton playQuizGame("Play!");
    playQuizGame.setPosition(playQuizGame.getCenterX(), quizMenuAbout.getBottom() + 30);

    //Quiz game start
    UILabel quizMenuStartText("You have 30 seconds. Are you ready?");
    quizMenuStartText.setPosition(quizMenuStartText.getCenterX(), quizMenuStartText.getCenterY() - 100);
    UIButton quizGameStartButton("Start");
    quizGameStartButton.setPosition(quizGameStartButton.getCenterX(), quizMenuStartText.getBottom() + 30);
    int quizGameCountdown = -1;
    UILabel countdownText("3");
    countdownText.setPosition(countdownText.getCenterX(), countdownText.getCenterY());
    
    //Quiz game
    std::vector<std::string> questions;
    std::vector<std::string> sectionA;
    std::vector<std::string> sectionB;
    std::vector<std::string> sectionC;
    std::vector<std::string> sectionD;
    std::vector<int> correctAnswer;

    questions.push_back("What is the purpose of EMIT?");
    sectionA.push_back("Earth’s Magnetic Investigation Tool");
    sectionB.push_back("Earth Surface Mineral Dust Source Investigation");
    sectionC.push_back("Environmental Monitoring and Information Technology");
    sectionD.push_back("Earth’s Microclimate Investigation Task");
    correctAnswer.push_back(B);

    questions.push_back("What is the EMIT’s goal?");
    sectionA.push_back("Exploring new planets");
    sectionB.push_back("Clearing dust from the atmosphere");
    sectionC.push_back("Analyzing outer space phenomena");
    sectionD.push_back("Investigating mineral dust sources on Earth’s Surface");
    correctAnswer.push_back(D);

    questions.push_back("Where does mineral dust come from?");
    sectionA.push_back("Oceans");
    sectionB.push_back("Arid land regions");
    sectionC.push_back("Space");
    sectionD.push_back("Forests");
    correctAnswer.push_back(B);

    questions.push_back("Which technology does EMIT use to measure atmospheric composition?");
    sectionA.push_back("Hyper-spectral imaging sensors");
    sectionB.push_back("Radar sensors");
    sectionC.push_back("Lidar sensors");
    sectionD.push_back("Advanced spectrometers");
    correctAnswer.push_back(D);

    questions.push_back("Which type of mineral dust can cause the world to warm up?");
    sectionA.push_back("Dark minerals");
    sectionB.push_back("Light minerals");
    sectionC.push_back("-");
    sectionD.push_back("-");
    correctAnswer.push_back(A);

    questions.push_back("What is EMIT's field of view?");
    sectionA.push_back("20 mil (32 km)");
    sectionB.push_back("100 mil (161 km)");
    sectionC.push_back("39 mil (72 km)");
    sectionD.push_back("73 mil (117 km)");
    correctAnswer.push_back(C);

    questions.push_back("Where was EMIT developed?");
    sectionA.push_back("Jet Propulsion Lab");
    sectionB.push_back("Latin Quarter Lab");
    sectionC.push_back("LSD Lab");
    sectionD.push_back("SNOLAB");
    correctAnswer.push_back(A);

    questions.push_back("When was EMIT launched?");
    sectionA.push_back("14th of July, 2021");
    sectionB.push_back("16th of July, 2022");
    sectionC.push_back("14th of July, 2022");
    sectionD.push_back("16th of July, 2021");
    correctAnswer.push_back(C);

    questions.push_back("Where was EMIT launched?");
    sectionA.push_back("Skylab");
    sectionB.push_back("International Space Station (ISS)");
    sectionC.push_back("Almaz");
    sectionD.push_back("Salute");
    correctAnswer.push_back(B);

    questions.push_back("Which greenhouse gases does EMIT detect?");
    sectionA.push_back("Ethane and nitrogen");
    sectionB.push_back("Propane");
    sectionC.push_back("Carbon monoxide");
    sectionD.push_back("Methane and Carbon dioxide");
    correctAnswer.push_back(D);

    falsesndb.loadFromFile("false.wav");
    falsesnd.setBuffer(falsesndb);
    truesndb.loadFromFile("true.wav");
    truesnd.setBuffer(truesndb);
    int remainingTime = 30;
    UILabel quizRemainingTimeText("");
    UILabel quizTotalPoints("");
    quizTotalPoints.setPosition(30, 30);
    UIButton endQuiz("End quiz");
    endQuiz.setPosition(endQuiz.getCenterX(), wndHeight - endQuiz.getHeight() - 30);
    int currentQuestion = 0, points = 0;

    UILabel summary("");
    summarysndb.loadFromFile("summary.wav");
    summarysnd.setBuffer(summarysndb);
    while (window.isOpen())
    {   
        mousePosition = sf::Mouse::getPosition(window);
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed)
                mouseDown = true;
            if (event.type == sf::Event::MouseButtonReleased)
                mouseDown = false;
        }
        window.clear(sf::Color::Black);
        switch (phase)
        {
        case PHASE_INTRO:
            if(frameCounter > 80)
                phase = PHASE_MAIN_MENU;
            window.draw(logoSprite);
            window.draw(logoTitle);
            break;
        case PHASE_MAIN_MENU:
            window.draw(bgSprite);
            titleLabel.draw();
            warningSystemButton.draw();
            gameMenuButton.draw();
            exitButton.draw();
            if(warningSystemButton.clicked()){
                phase = PHASE_WARNING_SYSTEM_INFO;
            }
            if(gameMenuButton.clicked()){
                phase = PHASE_GAME_MENU;
            }
            if(exitButton.clicked()){
                phase = PHASE_NONE;
            }
            break;
        case PHASE_WARNING_SYSTEM_INFO:
            window.draw(bgSprite);
            warningSystemTitle.draw();
            aboutWarningSystem.draw();
            backButton.draw();
            warningSystemDemoButton.draw();
            if(warningSystemDemoButton.clicked()){
                phase = PHASE_WARNING_SYSTEM_DEMO;
            }
            if(backButton.clicked()){
                phase = PHASE_MAIN_MENU;
            }
            break;
        case PHASE_WARNING_SYSTEM_DEMO:
            window.draw(bgSprite);
            warningSystemDemoTitle.draw();
            backButton.draw();
            warningSystemDemoDescription.draw();
            wsdl1.draw();
            wsdl2.draw();
            wsdl3.draw();
            if(wsdl1.clicked()){
                window.setVisible(0);
                selectedWarning = 1;
                frameCounter = 0;
            }
            if(wsdl2.clicked()){
                window.setVisible(0);
                selectedWarning = 2;
                frameCounter = 0;
            }
            if(wsdl3.clicked()){
                window.setVisible(0);
                selectedWarning = 3;
                frameCounter = 0;
            }
            if(backButton.clicked()){
                phase = PHASE_WARNING_SYSTEM_INFO;
            }
            break;
        case PHASE_WARNING_L1:
            window.draw(bgSprite);
            wltitle.draw();
            backButton.draw();
            wl1Msg.draw();
            if(backButton.clicked()){
                phase = PHASE_WARNING_SYSTEM_DEMO;
            }
            break;
        case PHASE_WARNING_L2:
            window.draw(bgSprite);
            wltitle.draw();
            backButton.draw();
            wl2Msg.draw();
            if(backButton.clicked()){
                phase = PHASE_WARNING_SYSTEM_DEMO;
            }
            break;
        case PHASE_WARNING_L3:
            wltitle.draw();
            backButton.draw();
            wl3Msg.draw();
            if(backButton.clicked()){
                phase = PHASE_WARNING_SYSTEM_DEMO;
            }
            break;
        case PHASE_GAME_MENU:
            window.draw(bgSprite);
            gameMenuTitle.draw();
            backButton.draw();
            aboutGame.draw();
            quizGameButton.draw();
            if(backButton.clicked()){
                phase = PHASE_MAIN_MENU;
            }
            if(quizGameButton.clicked()){
                phase = PHASE_QUIZ_GAME_MENU;
            }
            break;
        case PHASE_QUIZ_GAME_MENU:
            window.draw(bgSprite);
            backButton.draw();
            quizMenuTitle.draw();
            quizMenuAbout.draw();
            playQuizGame.draw();
            if(backButton.clicked()){
                phase = PHASE_GAME_MENU;
            }
            if(playQuizGame.clicked()){
                phase = PHASE_QUIZ_GAME_START;
            }
            break;
        case PHASE_QUIZ_GAME_START:
            window.draw(bgSprite);
            if(quizGameCountdown == -1){
                quizMenuTitle.draw();
                quizMenuStartText.draw();
                quizGameStartButton.draw();
                backButton.draw();
                if(backButton.clicked()){
                    phase = PHASE_QUIZ_GAME_MENU;
                }
                if(quizGameStartButton.clicked()){
                    cd.play();
                    quizGameCountdown = 4;
                    frameCounter = 0;
                }
            }else{
                if(frameCounter > 50){
                    frameCounter = 0;
                    quizGameCountdown--;
                    if(quizGameCountdown == 0){
                        quizGameCountdown = -1;
                        phase = PHASE_QUIZ_GAME;
                        loadq(currentQuestion);
                        quizTotalPoints.setString("Points: " + std::to_string(points));
                        remainingTime = 31;
                        frameCounter = 60;
                    }else{
                        if(quizGameCountdown == 1){
                            countdownText.setString("GO");
                            cde.play();
                        }else{
                            countdownText.setString(std::to_string(quizGameCountdown-1));
                            cd.play();
                        }   
                        countdownText.setPosition(countdownText.getCenterX(), countdownText.getCenterY());
                    }

                }
                countdownText.draw();
            }
            break;
        case PHASE_QUIZ_GAME:
            if(frameCounter >= 58){
                remainingTime--;
                frameCounter = 0;
                if(remainingTime < 10){
                    cd.play();
                }
                if(remainingTime >= 0){
                    quizRemainingTimeText.setString("Remaining time: " + std::to_string(remainingTime));
                    quizRemainingTimeText.setPosition(quizRemainingTimeText.getCenterX(), quizRemainingTimeText.getCenterY() + 100);
                }
                if(remainingTime < 0){
                    phase = PHASE_QUIZ_GAME_SUMMARY;
                    summary.setString("Collected points: " + std::to_string(points));
                    summarysnd.play();
                }
            }
            window.draw(bgSprite);
            quizText.draw();
            quizA.draw();
            quizB.draw();
            quizC.draw();
            quizD.draw();
            quizRemainingTimeText.draw();
            endQuiz.draw();
            quizTotalPoints.draw();
            if(endQuiz.clicked()){
                phase = PHASE_QUIZ_GAME_SUMMARY;
                summary.setString("Collected points: " + std::to_string(points));
                summarysnd.play();
            }
            if(quizA.clicked()){
                if(quizCorrectAnswer == A){
                    points++;
                    truesnd.play();
                }else{
                    falsesnd.play();
                }
                currentQuestion++;
                if(currentQuestion >= questions.size()){
                    currentQuestion = 0;
                }
                loadq(currentQuestion);
                quizTotalPoints.setString("Points: " + std::to_string(points));
            }
            if(quizB.clicked()){
                if(quizCorrectAnswer == B){
                    points++;
                    truesnd.play();
                }else{
                    falsesnd.play();
                }
                currentQuestion++;
                if(currentQuestion >= questions.size()){
                    currentQuestion = 0;
                }
                loadq(currentQuestion);
                quizTotalPoints.setString("Points: " + std::to_string(points));
            }
            if(quizC.clicked()){
                if(quizCorrectAnswer == C){
                    points++;
                    truesnd.play();
                }else{
                    falsesnd.play();
                }
                currentQuestion++;
                if(currentQuestion >= questions.size()){
                    currentQuestion = 0;
                }
                loadq(currentQuestion);
                quizTotalPoints.setString("Points: " + std::to_string(points));
            }
            if(quizD.clicked()){
                if(quizCorrectAnswer == D){
                    points++;
                    truesnd.play();
                }else{
                    falsesnd.play();
                }
                currentQuestion++;
                if(currentQuestion >= questions.size()){
                    currentQuestion = 0;
                }
                loadq(currentQuestion);
                quizTotalPoints.setString("Points: " + std::to_string(points));
            }
            break;
        case PHASE_QUIZ_GAME_SUMMARY:
            window.draw(bgSprite);
            summary.draw();
            backButton.draw();
            if(backButton.clicked()){
                phase = PHASE_QUIZ_GAME_MENU;
            }
            break;
        default:
            window.close();
            break;
        }
        window.display();
        frameCounter++;
        if(selectedWarning != 0){
            if(frameCounter > 180){
                switch (selectedWarning)
                {
                    case 1:
                        window.setVisible(1);
                        phase = PHASE_WARNING_L1;
                        break;
                    case 2:
                        window.setVisible(1);
                        l2s.play();
                        phase = PHASE_WARNING_L2;
                        break;
                    case 3:
                        window.setVisible(1);
                        l3s.play();
                        phase = PHASE_WARNING_L3;
                        break;
                    default:
                        break;
                }
                selectedWarning = 0;
            }
        }
        currentFrame++;
    }

    return EXIT_SUCCESS;
}
