#include "cocos2d.h"

#include <algorithm>
#include <cctype>
#include <cstdarg>
#include <cstring>
#include <exception>
#include <filesystem>
#include <fstream>
#include <limits>
#include <sstream>

namespace cocos2d {

const Vec2 Vec2::ZERO = Vec2(0.0f, 0.0f);

EventListenerTouchOneByOne* EventListenerTouchOneByOne::create() {
    auto* listener = new EventListenerTouchOneByOne();
    listener->autorelease();
    return listener;
}

void EventDispatcher::addEventListenerWithSceneGraphPriority(EventListenerTouchOneByOne* /*listener*/,
                                                             Ref* /*node*/) {}

Node::Node() { _eventDispatcher = new EventDispatcher(); }

Node::~Node() {
    if (_eventDispatcher) {
        _eventDispatcher->release();
        _eventDispatcher = nullptr;
    }
    for (auto* child : _children) {
        if (child) {
            child->_parent = nullptr;
            child->release();
        }
    }
    _children.clear();
}

bool Node::init() { return true; }

void Node::addChild(Node* child) {
    if (!child) {
        return;
    }
    if (child->_parent) {
        child->removeFromParent();
    }
    child->_parent = this;
    _children.push_back(child);
}

void Node::removeFromParent() {
    if (!_parent) {
        return;
    }
    auto* parent = _parent;
    _parent = nullptr;
    auto& siblings = parent->_children;
    siblings.erase(std::remove(siblings.begin(), siblings.end(), this), siblings.end());
    release();
}

void Node::setPosition(float x, float y) { setPosition(Vec2(x, y)); }

void Node::setPosition(const Vec2& pos) { _position = pos; }

Vec2 Node::convertToNodeSpace(const Vec2& world) const {
    return Vec2(world.x - _position.x, world.y - _position.y);
}

void Node::runAction(Action* action) {
    if (!action) {
        return;
    }
    action->retain();
    action->execute(this);
    action->release();
}

LayerColor* LayerColor::create(const Color4B& color, float width, float height) {
    auto* layer = new LayerColor();
    if (layer && layer->init()) {
        layer->setContentSize(Size(width, height));
        layer->_color = color;
        layer->autorelease();
        return layer;
    }
    delete layer;
    return nullptr;
}

Sprite* Sprite::create() {
    auto* sprite = new Sprite();
    if (sprite && sprite->init()) {
        sprite->setContentSize(Size(100.0f, 140.0f));
        sprite->autorelease();
        return sprite;
    }
    delete sprite;
    return nullptr;
}

Sprite* Sprite::create(const std::string& path) {
    auto* sprite = Sprite::create();
    if (sprite) {
        sprite->setTexture(path);
    }
    return sprite;
}

void Sprite::setTexture(const std::string& path) { _texturePath = path; }

Rect Sprite::getBoundingBox() const { return Rect(0.0f, 0.0f, getContentSize().width, getContentSize().height); }

MenuItemImage* MenuItemImage::create(const std::string& normalImage,
                                     const std::string& selectedImage,
                                     const std::string& disabledImage,
                                     const ccMenuCallback& callback) {
    auto* item = new MenuItemImage();
    if (item && item->init()) {
        item->_normalImage = normalImage;
        item->_selectedImage = selectedImage;
        item->_disabledImage = disabledImage;
        item->_callback = callback;
        item->autorelease();
        return item;
    }
    delete item;
    return nullptr;
}

void MenuItemImage::activate() {
    if (_enabled && _callback) {
        _callback(this);
    }
}

Menu* Menu::create(MenuItemImage* item, std::nullptr_t /*terminator*/) {
    auto* menu = new Menu();
    if (menu && menu->init()) {
        if (item) {
            menu->addChild(item);
        }
        menu->autorelease();
        return menu;
    }
    delete menu;
    return nullptr;
}

MoveTo::MoveTo(float duration, Vec2 targetPos) : _duration(duration), _targetPos(targetPos) {}

MoveTo* MoveTo::create(float duration, const Vec2& targetPos) {
    auto* action = new MoveTo(duration, targetPos);
    action->autorelease();
    return action;
}

void MoveTo::execute(Node* target) {
    if (target) {
        target->setPosition(_targetPos);
    }
}

FadeIn::FadeIn(float duration) : _duration(duration) {}

FadeIn* FadeIn::create(float duration) {
    auto* action = new FadeIn(duration);
    action->autorelease();
    return action;
}

void FadeIn::execute(Node* target) {
    if (target) {
        target->setOpacity(255);
    }
}

FadeOut::FadeOut(float duration) : _duration(duration) {}

FadeOut* FadeOut::create(float duration) {
    auto* action = new FadeOut(duration);
    action->autorelease();
    return action;
}

void FadeOut::execute(Node* target) {
    if (target) {
        target->setOpacity(0);
    }
}

CallFunc::CallFunc(std::function<void()> func) : _func(std::move(func)) {}

CallFunc* CallFunc::create(const std::function<void()>& func) {
    auto* action = new CallFunc(func);
    action->autorelease();
    return action;
}

void CallFunc::execute(Node* /*target*/) {
    if (_func) {
        _func();
    }
}

Sequence* Sequence::create(Action* first, ...) {
    auto* sequence = new Sequence();
    sequence->autorelease();

    va_list args;
    va_start(args, first);
    for (Action* action = first; action != nullptr; action = va_arg(args, Action*)) {
        action->retain();
        sequence->_actions.push_back(action);
    }
    va_end(args);

    return sequence;
}

void Sequence::execute(Node* target) {
    for (auto* action : _actions) {
        if (action) {
            action->execute(target);
            action->release();
        }
    }
    _actions.clear();
}

std::string StringUtils::format(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    va_list argsCopy;
    va_copy(argsCopy, args);
    const int len = std::vsnprintf(nullptr, 0, fmt, argsCopy);
    va_end(argsCopy);

    std::string result;
    if (len >= 0) {
        result.resize(static_cast<size_t>(len));
        std::vsnprintf(result.data(), result.size() + 1, fmt, args);
    }
    va_end(args);
    return result;
}

Director* Director::getInstance() {
    static Director instance;
    return &instance;
}

Director::Director() { _glView = std::make_unique<GLView>(); }

GLView* Director::getOpenGLView() { return _glView.get(); }

FileUtils* FileUtils::getInstance() {
    static FileUtils instance;
    return &instance;
}

std::string FileUtils::resolvePath(const std::string& filePath) const {
    namespace fs = std::filesystem;
#ifdef CARDGAME_RESOURCE_ROOT
    const fs::path rootPath = fs::path(CARDGAME_RESOURCE_ROOT);
#else
    const fs::path rootPath = fs::current_path();
#endif
    fs::path relativePath(filePath);
    if (fs::exists(relativePath)) {
        return relativePath.string();
    }
    const fs::path combined = rootPath / relativePath;
    return combined.string();
}

namespace {

class SimpleJsonParser {
public:
    SimpleJsonParser(const char* begin, const char* end) : _cur(begin), _end(end) {}

    Value parse() {
        skipWhitespace();
        Value value = parseValue();
        skipWhitespace();
        if (_cur != _end) {
            _error = true;
        }
        return value;
    }

    bool hasError() const { return _error; }

private:
    void skipWhitespace() {
        while (_cur < _end && (*_cur == ' ' || *_cur == '\n' || *_cur == '\r' || *_cur == '\t')) {
            ++_cur;
        }
    }

    bool consume(char expected) {
        if (_cur < _end && *_cur == expected) {
            ++_cur;
            return true;
        }
        _error = true;
        return false;
    }

    static int hexToInt(char ch) {
        if (ch >= '0' && ch <= '9') {
            return ch - '0';
        }
        if (ch >= 'a' && ch <= 'f') {
            return 10 + (ch - 'a');
        }
        if (ch >= 'A' && ch <= 'F') {
            return 10 + (ch - 'A');
        }
        return -1;
    }

    Value parseValue() {
        if (_cur >= _end) {
            _error = true;
            return Value();
        }
        char ch = *_cur;
        if (ch == '{') {
            return parseObject();
        }
        if (ch == '[') {
            return parseArray();
        }
        if (ch == '"') {
            return parseString();
        }
        if (ch == 't') {
            return parseLiteral("true", Value(true));
        }
        if (ch == 'f') {
            return parseLiteral("false", Value(false));
        }
        if (ch == 'n') {
            return parseLiteral("null", Value());
        }
        if (ch == '-' || (ch >= '0' && ch <= '9')) {
            return parseNumber();
        }
        _error = true;
        return Value();
    }

    Value parseLiteral(const char* literal, Value value) {
        for (const char* ptr = literal; *ptr != '\0'; ++ptr) {
            if (_cur >= _end || *_cur != *ptr) {
                _error = true;
                return Value();
            }
            ++_cur;
        }
        return value;
    }

    Value parseString() {
        if (!consume('"')) {
            return Value();
        }
        std::string result;
        while (_cur < _end) {
            char ch = *_cur++;
            if (ch == '"') {
                return Value(result);
            }
            if (ch == '\\') {
                if (_cur >= _end) {
                    _error = true;
                    return Value();
                }
                char esc = *_cur++;
                switch (esc) {
                case '"':
                    result.push_back('"');
                    break;
                case '\\':
                    result.push_back('\\');
                    break;
                case '/':
                    result.push_back('/');
                    break;
                case 'b':
                    result.push_back('\b');
                    break;
                case 'f':
                    result.push_back('\f');
                    break;
                case 'n':
                    result.push_back('\n');
                    break;
                case 'r':
                    result.push_back('\r');
                    break;
                case 't':
                    result.push_back('\t');
                    break;
                case 'u': {
                    if (_cur + 4 > _end) {
                        _error = true;
                        return Value();
                    }
                    int codePoint = 0;
                    for (int i = 0; i < 4; ++i) {
                        int digit = hexToInt(*_cur++);
                        if (digit < 0) {
                            _error = true;
                            return Value();
                        }
                        codePoint = (codePoint << 4) | digit;
                    }
                    if (codePoint <= 0x7F) {
                        result.push_back(static_cast<char>(codePoint));
                    } else {
                        // Fallback for non-ASCII characters.
                        result.push_back('?');
                    }
                    break;
                }
                default:
                    result.push_back(esc);
                    break;
                }
            } else {
                result.push_back(ch);
            }
        }
        _error = true;
        return Value();
    }

    Value parseNumber() {
        const char* start = _cur;
        if (_cur < _end && *_cur == '-') {
            ++_cur;
        }
        if (_cur < _end && *_cur == '0') {
            ++_cur;
        } else {
            if (_cur >= _end || !std::isdigit(static_cast<unsigned char>(*_cur))) {
                _error = true;
                return Value();
            }
            while (_cur < _end && std::isdigit(static_cast<unsigned char>(*_cur))) {
                ++_cur;
            }
        }
        bool isFloat = false;
        if (_cur < _end && *_cur == '.') {
            isFloat = true;
            ++_cur;
            if (_cur >= _end || !std::isdigit(static_cast<unsigned char>(*_cur))) {
                _error = true;
                return Value();
            }
            while (_cur < _end && std::isdigit(static_cast<unsigned char>(*_cur))) {
                ++_cur;
            }
        }
        if (_cur < _end && (*_cur == 'e' || *_cur == 'E')) {
            isFloat = true;
            ++_cur;
            if (_cur < _end && (*_cur == '+' || *_cur == '-')) {
                ++_cur;
            }
            if (_cur >= _end || !std::isdigit(static_cast<unsigned char>(*_cur))) {
                _error = true;
                return Value();
            }
            while (_cur < _end && std::isdigit(static_cast<unsigned char>(*_cur))) {
                ++_cur;
            }
        }
        std::string numberStr(start, _cur);
        try {
            if (!isFloat) {
                long long val = std::stoll(numberStr);
                if (val >= std::numeric_limits<int>::min() && val <= std::numeric_limits<int>::max()) {
                    return Value(static_cast<int>(val));
                }
                return Value(static_cast<double>(val));
            }
            double val = std::stod(numberStr);
            return Value(val);
        } catch (...) {
            _error = true;
            return Value();
        }
    }

    Value parseArray() {
        if (!consume('[')) {
            return Value();
        }
        std::vector<Value> vec;
        skipWhitespace();
        if (_cur < _end && *_cur == ']') {
            ++_cur;
            return Value(std::move(vec));
        }
        while (_cur < _end) {
            Value element = parseValue();
            vec.push_back(std::move(element));
            skipWhitespace();
            if (_cur < _end && *_cur == ',') {
                ++_cur;
                skipWhitespace();
                continue;
            }
            if (_cur < _end && *_cur == ']') {
                ++_cur;
                break;
            }
            _error = true;
            break;
        }
        return Value(std::move(vec));
    }

    Value parseObject() {
        if (!consume('{')) {
            return Value();
        }
        std::unordered_map<std::string, Value> map;
        skipWhitespace();
        if (_cur < _end && *_cur == '}') {
            ++_cur;
            return Value(std::move(map));
        }
        while (_cur < _end) {
            Value keyValue = parseString();
            if (hasError()) {
                return Value();
            }
            skipWhitespace();
            if (!consume(':')) {
                return Value();
            }
            skipWhitespace();
            Value value = parseValue();
            map[keyValue.asString()] = std::move(value);
            skipWhitespace();
            if (_cur < _end && *_cur == ',') {
                ++_cur;
                skipWhitespace();
                continue;
            }
            if (_cur < _end && *_cur == '}') {
                ++_cur;
                break;
            }
            _error = true;
            break;
        }
        return Value(std::move(map));
    }

    const char* _cur = nullptr;
    const char* _end = nullptr;
    bool _error = false;
};

} // namespace

FileUtils::ValueMap FileUtils::getValueMapFromFile(const std::string& filePath) {
    std::ifstream ifs(resolvePath(filePath));
    if (!ifs.is_open()) {
        return {};
    }
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    const std::string data = buffer.str();
    return getValueMapFromData(data.data(), data.size());
}

FileUtils::ValueMap FileUtils::getValueMapFromData(const char* data, size_t size) {
    if (!data || size == 0) {
        return {};
    }
    SimpleJsonParser parser(data, data + size);
    Value value = parser.parse();
    if (!parser.hasError() && value.getType() == Value::Type::MAP) {
        return value.asValueMap();
    }
    return {};
}

Value::Value() : _type(Type::NONE), _data(std::monostate{}) {}

Value::Value(int v) : _type(Type::INTEGER), _data(v) {}

Value::Value(float v) : _type(Type::FLOAT), _data(static_cast<double>(v)) {}

Value::Value(double v) : _type(Type::DOUBLE), _data(v) {}

Value::Value(bool v) : _type(Type::BOOLEAN), _data(v) {}

Value::Value(const char* v) : Value(std::string(v ? v : "")) {}

Value::Value(const std::string& v) : _type(Type::STRING), _data(v) {}

Value::Value(const std::vector<Value>& vec) : _type(Type::VECTOR), _data(vec) {}

Value::Value(std::vector<Value>&& vec) : _type(Type::VECTOR), _data(std::move(vec)) {}

Value::Value(const std::unordered_map<std::string, Value>& map)
    : _type(Type::MAP), _data(map) {}

Value::Value(std::unordered_map<std::string, Value>&& map)
    : _type(Type::MAP), _data(std::move(map)) {}

int Value::asInt() const {
    switch (_type) {
    case Type::INTEGER:
        return std::get<int>(_data);
    case Type::FLOAT:
    case Type::DOUBLE:
        return static_cast<int>(std::get<double>(_data));
    case Type::BOOLEAN:
        return std::get<bool>(_data) ? 1 : 0;
    default:
        return 0;
    }
}

float Value::asFloat() const {
    switch (_type) {
    case Type::INTEGER:
        return static_cast<float>(std::get<int>(_data));
    case Type::FLOAT:
    case Type::DOUBLE:
        return static_cast<float>(std::get<double>(_data));
    default:
        return 0.0f;
    }
}

double Value::asDouble() const {
    switch (_type) {
    case Type::INTEGER:
        return static_cast<double>(std::get<int>(_data));
    case Type::FLOAT:
    case Type::DOUBLE:
        return std::get<double>(_data);
    default:
        return 0.0;
    }
}

bool Value::asBool() const {
    if (_type == Type::BOOLEAN) {
        return std::get<bool>(_data);
    }
    if (_type == Type::INTEGER) {
        return std::get<int>(_data) != 0;
    }
    return false;
}

std::string Value::asString() const {
    if (_type == Type::STRING) {
        return std::get<std::string>(_data);
    }
    return {};
}

const std::vector<Value>& Value::asValueVector() const { return std::get<std::vector<Value>>(_data); }

std::vector<Value>& Value::asValueVector() { return std::get<std::vector<Value>>(_data); }

const std::unordered_map<std::string, Value>& Value::asValueMap() const {
    return std::get<std::unordered_map<std::string, Value>>(_data);
}

std::unordered_map<std::string, Value>& Value::asValueMap() {
    return std::get<std::unordered_map<std::string, Value>>(_data);
}

Value& Value::operator=(const std::vector<Value>& vec) {
    _type = Type::VECTOR;
    _data = vec;
    return *this;
}

Value& Value::operator=(int v) {
    _type = Type::INTEGER;
    _data = v;
    return *this;
}

Value& Value::operator=(float v) {
    _type = Type::FLOAT;
    _data = static_cast<double>(v);
    return *this;
}

Value& Value::operator=(double v) {
    _type = Type::DOUBLE;
    _data = v;
    return *this;
}

Value& Value::operator=(bool v) {
    _type = Type::BOOLEAN;
    _data = v;
    return *this;
}

Value& Value::operator=(const std::string& v) {
    _type = Type::STRING;
    _data = v;
    return *this;
}

Value& Value::operator=(const char* v) {
    return (*this = std::string(v ? v : ""));
}

Value& Value::operator=(std::vector<Value>&& vec) {
    _type = Type::VECTOR;
    _data = std::move(vec);
    return *this;
}

Value& Value::operator=(const std::unordered_map<std::string, Value>& map) {
    _type = Type::MAP;
    _data = map;
    return *this;
}

Value& Value::operator=(std::unordered_map<std::string, Value>&& map) {
    _type = Type::MAP;
    _data = std::move(map);
    return *this;
}

void log(const char* format, ...) {
    va_list args;
    va_start(args, format);
    std::vfprintf(stdout, format, args);
    std::fprintf(stdout, "\n");
    va_end(args);
}

} // namespace cocos2d

