#pragma once
#include <any>
#include <string>
#include <vector>
#include <map>
#include "object.hpp"

using namespace std;

namespace Alchemy {
    
    class Message {
        public:
            string channel;
            string subject;
            any value;

            Message(string channel, string subject, any value) {
                this->channel = channel;
                this->subject = subject;
                this->value = value;
            }
    };

    class IActor : public Object {
        public:
            IActor () : Object () {};
            virtual void receive(string, any) {};
            ~IActor() {};
    };

    class MessageBus {
        private:
            vector<Message> messages;
            map<string, vector<IActor*>> channels;

        public:
            MessageBus () {};

            void addChannel(string channel) {
                this->channels.insert(
                    pair<string, vector<IActor*>>(channel, vector<IActor*>())
                );
            };

            void publish(string channel, string message, any value) {
                auto receivers = this->channels.find(channel);
                if (receivers != this->channels.end()) {
                    auto receiverList = receivers->second;
                    for (auto iter = receiverList.begin(); iter != receiverList.end(); iter++) {
                        (*iter)->receive(message, value);
                    }
                } else {
                    // TODO: Warn unknown channel / no receivers
                }
            };

            void subscribe(string channel, IActor* receiver) {
                auto receivers = this->channels.find(channel);
                if (receivers == this->channels.end()) {
                    this->addChannel(channel);
                }
                this->channels[channel].push_back(receiver);
            };

            void unsubscribe(IActor* actor) {
                for (auto iterChan = this->channels.begin(); iterChan != this->channels.end(); iterChan++) {
                    for (auto iter = iterChan->second.begin(); iter != iterChan->second.end(); iter++) {
                        if (*(*iter) == (*actor)) {
                            iterChan->second.erase(iter);
                        }
                    }
                }
            }

            ~MessageBus() {};
    };

    class Actor : public IActor {
        protected: 
            MessageBus* messageBus;
        public:
            Actor (MessageBus* bus) : IActor() {
                this->messageBus = bus;
            };

            virtual void receive (string subject, any value) {}

            void send (string channel, string subject, any value) {
                this->messageBus->publish(channel, subject, value);
            }

            void subscribe(string channel) {
                this->messageBus->subscribe(channel, this);
            }

            ~Actor() {
                this->messageBus->unsubscribe(this);
            }
    };
};