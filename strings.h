#ifndef _STRINGS_GRAPHICS_H
#define _STRINGS_GRAPHICS_H

#include "general.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

const int MAX_INCR_SIZE = 32;

enum {
    S_ALL,
    S_NUM,
    S_ALPH
};

class Strings {
    private:
        unsigned int length;  //Actually the max length
        std::string str;
        SDL_Surface* inputS;
    public:
        std::string getStr() {
            return str;
        }
        void setStr(std::string newStr){
            str = newStr;
        }
        SDL_Surface* getSurface(){
            return inputS;
        }
        void setSurface(SDL_Surface* newSFC = NULL){
            inputS = newSFC;
        }
        Strings(){
            str = "";
            inputS = NULL;
            SDL_EnableUNICODE(SDL_ENABLE);
            length = 255;
        }


        //defines a specific max length for the string, in case 255 isn't good
        Strings(int maxLength){
            str = "";
            inputS = NULL;
            length = maxLength;
            SDL_EnableUNICODE(SDL_ENABLE);
        }

        ~Strings(){
            SDL_FreeSurface(inputS);
            SDL_EnableUNICODE(SDL_DISABLE);
        }

      void input(int mode = 0)
      //mode values:
      //0 - all input enabled
      //1 - only numbers + spaces
      //2 - only letters + spaces
      {
        if (event.type == SDL_KEYDOWN) {
          std::string temp = str;
          if (str.length() <= length) {
            if (((event.key.keysym.unicode >= (Uint16)'0') &&
                 (event.key.keysym.unicode <= (Uint16)'9')) &&
                (mode == 0 || mode == 1)) {
              str += (char)event.key.keysym.unicode;
            }
            else if (((event.key.keysym.unicode >= (Uint16)'A') &&
                  (event.key.keysym.unicode <= (Uint16)'Z') &&
                  (mode == 0 || mode == 2))) {
              str += (char)event.key.keysym.unicode;
            }
            else if (((event.key.keysym.unicode >= (Uint16)'a') &&
                  (event.key.keysym.unicode <= (Uint16)'z') &&
                  (mode == 0 || mode == 2))) {
              str += (char)event.key.keysym.unicode;
            }
            else if (event.key.keysym.unicode == (Uint16)' ') {
              str += (char)event.key.keysym.unicode;
            }
          }
          if ((event.key.keysym.sym == SDLK_BACKSPACE) && (str.length() != 0)) {
            str.erase(str.length() - 1);
          }
          if (str != temp) {
            SDL_FreeSurface(inputS);
            inputS = TTF_RenderText_Solid(font, str.c_str(), textC);
          }
        }
      }

          //in case the max size ever needs to increase
        void expand() {
            length += MAX_INCR_SIZE;
        }

};


#endif
