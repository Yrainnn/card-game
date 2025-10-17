#pragma once

#include <algorithm>
#include <cstdarg>
#include <cstdint>
#include <cstdio>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#define CC_CALLBACK_0(__selector__, __target__) std::bind(&__selector__, __target__)
#define CC_CALLBACK_1(__selector__, __target__)                                                            \
    std::bind(&__selector__, __target__, std::placeholders::_1)
#define CC_CALLBACK_2(__selector__, __target__)                                                            \
    std::bind(&__selector__, __target__, std::placeholders::_1, std::placeholders::_2)

namespace cocos2d {

class Ref {
public:
    Ref() = default;
    virtual ~Ref() = default;

    void retain() {}
    void release() { delete this; }
    Ref* autorelease() { return this; }
};

struct Vec2 {
    float x = 0.0f;
    float y = 0.0f;

    Vec2() = default;
    Vec2(float xx, float yy) : x(xx), y(yy) {}

    static const Vec2 ZERO;
};

struct Size {
    float width = 0.0f;
    float height = 0.0f;

    Size() = default;
    Size(float w, float h) : width(w), height(h) {}
};

struct Rect {
    float x = 0.0f;
    float y = 0.0f;
    float width = 0.0f;
    float height = 0.0f;

    Rect() = default;
    Rect(float xx, float yy, float ww, float hh) : x(xx), y(yy), width(ww), height(hh) {}

    bool containsPoint(const Vec2& point) const {
        return point.x >= x && point.x <= x + width && point.y >= y && point.y <= y + height;
    }
};

struct Color4B {
    uint8_t r = 0;
    uint8_t g = 0;
    uint8_t b = 0;
    uint8_t a = 0;

    Color4B() = default;
    Color4B(uint8_t rr, uint8_t gg, uint8_t bb, uint8_t aa) : r(rr), g(gg), b(bb), a(aa) {}
};

class Event : public Ref {};

class Touch : public Ref {
public:
    void setLocation(const Vec2& location) { _location = location; }
    Vec2 getLocation() const { return _location; }

private:
    Vec2 _location;
};

class EventListenerTouchOneByOne : public Ref {
public:
    using TouchCallback = std::function<bool(Touch*, Event*)>;
    using TouchEndedCallback = std::function<void(Touch*, Event*)>;

    static EventListenerTouchOneByOne* create();

    void setSwallowTouches(bool swallow) { _swallowTouches = swallow; }

    TouchCallback onTouchBegan;
    TouchEndedCallback onTouchEnded;

private:
    bool _swallowTouches = false;
};

class EventDispatcher : public Ref {
public:
    void addEventListenerWithSceneGraphPriority(EventListenerTouchOneByOne* listener, Ref* /*node*/);
};

class Action : public Ref {
public:
    virtual void execute(class Node* target) = 0;
};

class Node : public Ref {
public:
    Node();
    ~Node() override;

    virtual bool init();

    void addChild(Node* child);
    void removeFromParent();

    void setPosition(float x, float y);
    void setPosition(const Vec2& pos);
    Vec2 getPosition() const { return _position; }
    Vec2 convertToNodeSpace(const Vec2& world) const;

    void setContentSize(const Size& size) { _contentSize = size; }
    Size getContentSize() const { return _contentSize; }

    void setOpacity(uint8_t opacity) { _opacity = opacity; }
    uint8_t getOpacity() const { return _opacity; }

    void setVisible(bool visible) { _visible = visible; }
    bool isVisible() const { return _visible; }

    void runAction(Action* action);

protected:
    EventDispatcher* _eventDispatcher = nullptr;

private:
    Vec2 _position;
    Size _contentSize;
    uint8_t _opacity = 255;
    bool _visible = true;
    Node* _parent = nullptr;
    std::vector<Node*> _children;
};

class LayerColor : public Node {
public:
    static LayerColor* create(const Color4B& color, float width, float height);

private:
    Color4B _color;
};

class Sprite : public Node {
public:
    static Sprite* create();
    static Sprite* create(const std::string& path);

    void setTexture(const std::string& path);
    Rect getBoundingBox() const;

private:
    std::string _texturePath;
};

using ccMenuCallback = std::function<void(Ref*)>;

class MenuItemImage : public Node {
public:
    static MenuItemImage* create(const std::string& normalImage,
                                 const std::string& selectedImage,
                                 const std::string& disabledImage,
                                 const ccMenuCallback& callback);

    void setEnabled(bool enabled) { _enabled = enabled; }
    bool isEnabled() const { return _enabled; }
    void activate();

private:
    ccMenuCallback _callback;
    bool _enabled = true;
    std::string _normalImage;
    std::string _selectedImage;
    std::string _disabledImage;
};

class Menu : public Node {
public:
    static Menu* create(MenuItemImage* item, std::nullptr_t terminator);
};

class MoveTo : public Action {
public:
    static MoveTo* create(float duration, const Vec2& targetPos);
    void execute(Node* target) override;

private:
    MoveTo(float duration, Vec2 targetPos);

    float _duration;
    Vec2 _targetPos;
};

class FadeIn : public Action {
public:
    static FadeIn* create(float duration);
    void execute(Node* target) override;

private:
    explicit FadeIn(float duration);
    float _duration;
};

class FadeOut : public Action {
public:
    static FadeOut* create(float duration);
    void execute(Node* target) override;

private:
    explicit FadeOut(float duration);
    float _duration;
};

class CallFunc : public Action {
public:
    static CallFunc* create(const std::function<void()>& func);
    void execute(Node* target) override;

private:
    explicit CallFunc(std::function<void()> func);
    std::function<void()> _func;
};

class Sequence : public Action {
public:
    static Sequence* create(Action* first, ...);
    void execute(Node* target) override;

private:
    std::vector<Action*> _actions;
};

class StringUtils {
public:
    static std::string format(const char* fmt, ...);
};

class GLView : public Ref {
public:
    Size getFrameSize() const { return _frameSize; }
    void setFrameSize(float width, float height) { _frameSize = Size(width, height); }

private:
    Size _frameSize = Size(1080.0f, 2080.0f);
};

class Director : public Ref {
public:
    static Director* getInstance();

    GLView* getOpenGLView();

private:
    Director();

    std::unique_ptr<GLView> _glView;
};

class FileUtils : public Ref {
public:
    static FileUtils* getInstance();

    using ValueMap = std::unordered_map<std::string, class Value>;

    ValueMap getValueMapFromFile(const std::string& filePath);
    ValueMap getValueMapFromData(const char* data, size_t size);

private:
    FileUtils() = default;
    std::string resolvePath(const std::string& filePath) const;
};

class Value {
public:
    enum class Type {
        NONE,
        INTEGER,
        FLOAT,
        DOUBLE,
        BOOLEAN,
        STRING,
        VECTOR,
        MAP
    };

    Value();
    Value(int v);
    Value(float v);
    Value(double v);
    Value(bool v);
    Value(const char* v);
    Value(const std::string& v);
    Value(const std::vector<Value>& vec);
    Value(std::vector<Value>&& vec);
    Value(const std::unordered_map<std::string, Value>& map);
    Value(std::unordered_map<std::string, Value>&& map);

    Type getType() const { return _type; }

    int asInt() const;
    float asFloat() const;
    double asDouble() const;
    bool asBool() const;
    std::string asString() const;
    const std::vector<Value>& asValueVector() const;
    std::vector<Value>& asValueVector();
    const std::unordered_map<std::string, Value>& asValueMap() const;
    std::unordered_map<std::string, Value>& asValueMap();

    Value& operator=(int v);
    Value& operator=(float v);
    Value& operator=(double v);
    Value& operator=(bool v);
    Value& operator=(const std::string& v);
    Value& operator=(const char* v);
    Value& operator=(const std::vector<Value>& vec);
    Value& operator=(std::vector<Value>&& vec);
    Value& operator=(const std::unordered_map<std::string, Value>& map);
    Value& operator=(std::unordered_map<std::string, Value>&& map);

private:
    Type _type = Type::NONE;
    std::variant<std::monostate,
                 int,
                 double,
                 bool,
                 std::string,
                 std::vector<Value>,
                 std::unordered_map<std::string, Value>> _data;
};

void log(const char* format, ...);

using ValueMap = std::unordered_map<std::string, Value>;
using ValueVector = std::vector<Value>;

} // namespace cocos2d

