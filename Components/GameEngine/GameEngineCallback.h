#ifndef GAMEENGINECALLBACK_H
#define GAMEENGINECALLBACK_H

#include <iostream>
#include <string>
#include "../Player/headers/Player.h"
#include "../Components/headers/Factory.h"

class GameEngineCallback{
public:
    GameEngineCallback();
    ~GameEngineCallback();

    // Prints board of a player
    // void playerBoardUpdate(Player* player) const;
    void playerBoardUpdate(Player** players, int numPlayers) const;

    // Prints whose turn is it now
    void playerTurnUpdate(const std::string playerName) const;

    // Asks the player which central factory they wish to move to, if applicable.
    void centralFactoryChoice(std::vector<std::shared_ptr<Tile>> tiles) const;

    // Prints the results of the turn (eg. turn success , error)
    void playerTurnResult(const std::string turnResult) const;

    // Prints results of the end of round (board + points)
    void playerEndOfRoundResult(Player** players, int numPlayers) const;

    // Prints results of the end of game (board + points + winner)
    void playerEndOfGameResult(Player** players, int numPlayers) const;

    // Prints board components (factories)
    void boardComponentUpdate(Factory** factory, int numFactories, int numCentral) const;

    // used for formatting strings to make their spaces work with the terminal
    std::string formatString(std::string playerName, int numSpaces) const;

    // method for output to user when replaying a turn
    void replayTurn(Player* player, std::string playerCommand) const;

    // method for output to user when replaying a central choice
    void replayCentral(Player* player) const;

    // method for output to user when replaying a central choice
    void replayCentralChosen(Player* player, std::string usersChoice) const;

    // method for outputting to user asking for next string
    void replayNext() const;

    // method for outputting to user when replay has ended.
    void replayEnd() const;

    // Prints when the game is over due to no more tiles.
    void tilesRanOut() const;

    // method for asking user about saving the game when it ends
    void endOfGameSaveChoice() const;

    // method for asking user for a filename to save as.
    void endOfGameSaveName() const;

private:


};

#endif // GAMEENGINECALLBACK_H