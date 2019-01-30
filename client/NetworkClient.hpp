//
// Created by agrmv on 12.01.19.
//

#ifndef TICTACTOE_CLIENT_NETWORKCLIENT_HPP
#define TICTACTOE_CLIENT_NETWORKCLIENT_HPP

#include <algorithm>
#include <functional>
#include <deque>
#include "CClient.hpp"
#include "CInPacket.hpp"
#include "COutPacket.hpp"
#include <thread>

template <typename T>
struct HandlerIdentifier {
    using value = typename std::deque<T>::const_iterator;
    HandlerIdentifier(const value& init)
    : iterator { std::make_unique<value>(init) } {}

    /*HandlerIdentifier<T>& operator=(const HandlerIdentifier<T>& source) {
        iterator = std::make_unique<value>(*source.iterator);
        return *this;
    }*/

    bool valid() const {
        return iterator.get() != nullptr;
    }

    value reset() {
        value result {*iterator};
        iterator.reset();
        return result;
    }
private:
    std::unique_ptr<value> iterator;
};

template <typename Ret, typename... Args>
bool operator==(const std::function<Ret(Args...)>& left, const std::function<Ret(Args...)>& right) {
    return left.template target<void>() == right.template target<void>();
}

class NetworkClient {
public:
    using packet_handler = std::function<void(const CInPacket&)>;
    struct stPacketTrigger {
        ePackets type;
        packet_handler callback;

        void operator()(const CInPacket& packet) {
            std::thread(callback, std::cref(packet)).detach();
        }
    };
    using handler_identifier = HandlerIdentifier<stPacketTrigger>;

    NetworkClient() : listener { &NetworkClient::listen, this } {}

    void send(const COutPacket& packet) {
        packet.send(client);
    }

    handler_identifier createHandler(ePackets packet_type, const packet_handler& callback) {
        static auto delegate = [&](auto& iter) {
            return iter.type == packet_type
                && iter.callback == callback;
        };
        if (std::find_if(callbacks.begin(), callbacks.end(), delegate) != callbacks.end())
            throw std::logic_error("This callback for this package is already defined.");
        callbacks.push_back({ .type = packet_type, .callback = callback });
        return callbacks.cend() - 1;
    }

    void deleteHandler(handler_identifier& identifier) {
        if (!identifier.valid())
            return;
        callbacks.erase(identifier.reset());
    }

    void run(const sf::IpAddress& address, uint16_t port) {
        client.connect(address, port);
        listener.launch();
    }

    void stop() {
        client.disconnect();
        listener.wait();//terminate();
    }

    ~NetworkClient() {
        stop();
    }
private:
    void listen() {
        CInPacket packet;
        while(packet.read(client)) {
            for (auto& handler : callbacks) {
                if (handler.type == packet.getPacketType())
                    handler(packet);
            }
        }
    }

    CClient client;
    sf::Thread listener;
    std::deque<stPacketTrigger> callbacks;
};

#endif //TICTACTOE_CLIENT_NETWORKCLIENT_HPP