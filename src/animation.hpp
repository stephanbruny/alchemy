#pragma once
#include <string>
#include <map>
#include <SFML/Graphics.hpp>
#include <functional>
#include "../lib/rapidjson/rapidjson.h"
#include "../lib/rapidjson/document.h"
#include <iostream>
#include <fstream>
#include <streambuf>

using namespace std;

namespace Alchemy {
    
    class AnimationFrame {
        public:
            int x;
            int y;
            int width;
            int height;
            float duration;
    };

    class Animation {
        public:
            map<string, vector<AnimationFrame>> frames;
            AnimationFrame* currentFrame;
            vector<AnimationFrame> *currentAnimations;

            float frameTime;

            unsigned int currentIndex;

            Animation() {
                this->frameTime = 0;
                this->currentFrame = NULL;
                this->currentAnimations = NULL;
            }

            Animation (Animation* anim) {
                this->frameTime = 0;
                this->currentIndex = 0;
                this->currentFrame = NULL;
                this->currentAnimations = NULL;
                this->frames = map<string, vector<AnimationFrame>>(anim->frames);

                for (auto iter = this->frames.begin(); iter != this->frames.end(); iter++) {
                    cout << "State: " << iter->first << endl;
                    for (auto i = iter->second.begin(); i != iter->second.end(); i++) {
                        cout << (*i).duration << endl;
                    }
                }
            }

            void InsertFrame(string state, int x, int y, int w, int h, float d) {
                // cout << "Frame: " << state << "=" << x << ", " << y << ":" << w << "," << h << "=>" << d << endl;
                auto frameList = this->frames.find(state);
                if (frameList == this->frames.end()) {
                    this->frames.insert(pair<string, vector<AnimationFrame>>(
                        state,
                        vector<AnimationFrame> {  
                            AnimationFrame {
                                x, y, w, h, d
                            }
                        }
                    ));
                    return;
                }
                frameList->second.push_back(
                    AnimationFrame {
                        x, y, w, h, d
                    }
                );
            }

            void setState(string state) {
                auto frameList = this->frames.find(state);
                if (frameList == this->frames.end()) return;
                auto frame = frameList->second.at(0);
                this->currentAnimations = &frameList->second;
                this->currentFrame = &frame;
                this->currentIndex = 0;
                this->frameTime = 0;
                this->setNextFrame(0);
            }

            void setNextFrame (int index) {
                if (NULL == this->currentAnimations) return;
                this->currentIndex = index;
                this->frameTime = 0;
                if (this->currentIndex > this->currentAnimations->size() - 1) {
                    this->currentIndex = 0;
                    this->currentFrame = &this->currentAnimations->at(0);
                }
                this->currentFrame = &this->currentAnimations->at(this->currentIndex);
            }

            void setNextFrame () {
                setNextFrame(this->currentIndex + 1);
            }

            /**
             * returns true when animation changed
             */
            bool onUpdate(float dt) {
                if (NULL == this->currentFrame || NULL == this->currentAnimations)
                    return false;
                this->frameTime += dt;
                if (this->frameTime > this->currentFrame->duration) {
                    this->setNextFrame();
                    return true;
                }
                return false;
            }

            AnimationFrame* getCurrentFrame () {
                return this->currentFrame;
            };
    };

    class Animations {
        public:
            map<string, Animation*> animations;

            Animations () {}

            Animation* load (string filename) {
                auto item = this->animations.find(filename);
                if (item != this->animations.end()) {
                    return item->second;
                }
                ifstream file(filename);
                string content(
                    (istreambuf_iterator<char>(file)),
                    (istreambuf_iterator<char>())
                );
                Animation* result = new Animation();
                rapidjson::Document doc;
                doc.Parse(content.c_str());
                for (auto iter = doc.MemberBegin(); iter != doc.MemberEnd(); ++iter) {
                    string keyName(iter->name.GetString());
                    auto array = iter->value.GetArray();
                    for (auto item = array.Begin(); item != array.End(); ++item) {
                        auto obj = item->GetObject();
                        result->InsertFrame(
                            keyName,
                            obj["x"].GetInt(),
                            obj["y"].GetInt(),
                            obj["w"].GetInt(),
                            obj["h"].GetInt(),
                            obj["duration"].GetFloat()
                        );
                    }
                }
                this->animations.insert(pair<string, Animation*>(filename, result));
                return load(filename);
            }
    };
};