#include <v8.h>
#include <linux/input.h>

#include <iostream>

#include <node_buffer.h>
#include <node_events.h>

using namespace v8;
using namespace node;


inline
double
tv2v8(timeval const& tv) {
    return 1000 * (tv.tv_sec + (static_cast<double>(tv.tv_usec) / 1000000));
}

class Source : public EventEmitter {
    public:
        static
        void
        init(Handle<Object> target) {
            HandleScope scope;
            Local<FunctionTemplate> t = FunctionTemplate::New(New);
            t->Inherit(EventEmitter::constructor_template);
            t->InstanceTemplate()->SetInternalFieldCount(1);

            NODE_SET_PROTOTYPE_METHOD(t, "dispatchEventsInBuffer", DispatchEventsInBuffer);

            input_event_symbol = NODE_PSYMBOL("input_event");
            key_symbol = NODE_PSYMBOL("key");
            key_pressed_symbol = NODE_PSYMBOL("keyPressed");
            key_released_symbol = NODE_PSYMBOL("keyReleased");

            time_symbol = NODE_PSYMBOL("time");
            type_symbol = NODE_PSYMBOL("type");
            code_symbol = NODE_PSYMBOL("code");
            value_symbol = NODE_PSYMBOL("value");

            target->Set(String::NewSymbol("Source"), t->GetFunction());
        }
    private:
        enum { InputEventSize = sizeof(input_event) };

        static
        Handle<Value>
        New(Arguments const& args) {
            HandleScope scope;

            Source * src = new Source();
            src->Wrap(args.This());
            return args.This();
        }

        static
        Handle<Value>
        DispatchEventsInBuffer(Arguments const& args) {
            HandleScope scope;
            if (args.Length() != 1) {
                return ThrowException(Exception::Error(String::New("expected 1 argument")));
            }
            /* probably removed by the optimizer because node itself never calls it
            if (! Buffer::HasInstance(args[0])) {
                return ThrowException(Exception::Error(String::New("argument must be a buffer")));
            }
            */
            Buffer * buffer = ObjectWrap::Unwrap<Buffer>(args[0]->ToObject());
            Source * src = ObjectWrap::Unwrap<Source>(args.This());

            src->dispatch_events_in_buffer(buffer);

            return Undefined();
        }

        void
        dispatch_events_in_buffer(Buffer * buffer) {
            input_event * events = reinterpret_cast<input_event*>(buffer->data());
            size_t num_events = buffer->length() / InputEventSize;
            for (size_t i = 0; i < num_events; ++i) {
                input_event const& event = events[i];
                Local<Object> jsevent = Object::New();
                jsevent->Set(time_symbol, Date::New(tv2v8(events[i].time)));
                jsevent->Set(type_symbol, Integer::New(events[i].type));
                jsevent->Set(code_symbol, Integer::New(events[i].code));
                jsevent->Set(value_symbol, Integer::New(events[i].value));
                Local<Value> args[1];
                args[0] = jsevent;
                Emit(input_event_symbol, 1, args);

                if (events[i].type == EV_KEY) {
                    Emit(key_symbol, 1, args);
                    if (events[i].value == 1) {
                        Emit(key_pressed_symbol, 1, args);
                    } else {
                        Emit(key_released_symbol, 1, args);
                    }
                }
            }            
        }
        
        static Persistent<String> input_event_symbol;
        static Persistent<String> key_symbol;
        static Persistent<String> key_pressed_symbol;
        static Persistent<String> key_released_symbol;

        static Persistent<String> time_symbol;
        static Persistent<String> type_symbol;
        static Persistent<String> code_symbol;
        static Persistent<String> value_symbol;
};

Persistent<String> Source::input_event_symbol;
Persistent<String> Source::key_symbol;
Persistent<String> Source::key_pressed_symbol;
Persistent<String> Source::key_released_symbol;

Persistent<String> Source::time_symbol;
Persistent<String> Source::type_symbol;
Persistent<String> Source::code_symbol;
Persistent<String> Source::value_symbol;

extern "C"
void
init(Handle<Object> target) {
    HandleScope scope;
    Source::init(target);

    NODE_DEFINE_CONSTANT(target, EV_SYN);
    NODE_DEFINE_CONSTANT(target, EV_KEY);
    NODE_DEFINE_CONSTANT(target, EV_REL);
    NODE_DEFINE_CONSTANT(target, EV_ABS);
    NODE_DEFINE_CONSTANT(target, EV_MSC);
    NODE_DEFINE_CONSTANT(target, EV_SW);
    NODE_DEFINE_CONSTANT(target, EV_LED);
    NODE_DEFINE_CONSTANT(target, EV_SND);
    NODE_DEFINE_CONSTANT(target, EV_REP);
    NODE_DEFINE_CONSTANT(target, EV_FF);
    NODE_DEFINE_CONSTANT(target, EV_PWR);
    NODE_DEFINE_CONSTANT(target, EV_FF_STATUS);
    NODE_DEFINE_CONSTANT(target, EV_MAX);
    NODE_DEFINE_CONSTANT(target, EV_CNT);

    NODE_DEFINE_CONSTANT(target, BTN_MISC);
    NODE_DEFINE_CONSTANT(target, BTN_0);
    NODE_DEFINE_CONSTANT(target, BTN_1);
    NODE_DEFINE_CONSTANT(target, BTN_2);
    NODE_DEFINE_CONSTANT(target, BTN_3);
    NODE_DEFINE_CONSTANT(target, BTN_4);
    NODE_DEFINE_CONSTANT(target, BTN_5);
    NODE_DEFINE_CONSTANT(target, BTN_6);
    NODE_DEFINE_CONSTANT(target, BTN_7);
    NODE_DEFINE_CONSTANT(target, BTN_8);
    NODE_DEFINE_CONSTANT(target, BTN_9);

    NODE_DEFINE_CONSTANT(target, BTN_MOUSE);
    NODE_DEFINE_CONSTANT(target, BTN_LEFT);
    NODE_DEFINE_CONSTANT(target, BTN_RIGHT);
    NODE_DEFINE_CONSTANT(target, BTN_MIDDLE);
    NODE_DEFINE_CONSTANT(target, BTN_SIDE);
    NODE_DEFINE_CONSTANT(target, BTN_EXTRA);
    NODE_DEFINE_CONSTANT(target, BTN_FORWARD);
    NODE_DEFINE_CONSTANT(target, BTN_BACK);
    NODE_DEFINE_CONSTANT(target, BTN_TASK);

    NODE_DEFINE_CONSTANT(target, BTN_JOYSTICK);
    NODE_DEFINE_CONSTANT(target, BTN_TRIGGER);
    NODE_DEFINE_CONSTANT(target, BTN_THUMB);
    NODE_DEFINE_CONSTANT(target, BTN_THUMB2);
    NODE_DEFINE_CONSTANT(target, BTN_TOP);
    NODE_DEFINE_CONSTANT(target, BTN_TOP2);
    NODE_DEFINE_CONSTANT(target, BTN_PINKIE);
    NODE_DEFINE_CONSTANT(target, BTN_BASE);
    NODE_DEFINE_CONSTANT(target, BTN_BASE2);
    NODE_DEFINE_CONSTANT(target, BTN_BASE3);
    NODE_DEFINE_CONSTANT(target, BTN_BASE4);
    NODE_DEFINE_CONSTANT(target, BTN_BASE5);
    NODE_DEFINE_CONSTANT(target, BTN_BASE6);
    NODE_DEFINE_CONSTANT(target, BTN_DEAD);

    NODE_DEFINE_CONSTANT(target, BTN_GAMEPAD);
    NODE_DEFINE_CONSTANT(target, BTN_A);
    NODE_DEFINE_CONSTANT(target, BTN_B);
    NODE_DEFINE_CONSTANT(target, BTN_C);
    NODE_DEFINE_CONSTANT(target, BTN_X);
    NODE_DEFINE_CONSTANT(target, BTN_Y);
    NODE_DEFINE_CONSTANT(target, BTN_Z);
    NODE_DEFINE_CONSTANT(target, BTN_TL);
    NODE_DEFINE_CONSTANT(target, BTN_TR);
    NODE_DEFINE_CONSTANT(target, BTN_TL2);
    NODE_DEFINE_CONSTANT(target, BTN_TR2);
    NODE_DEFINE_CONSTANT(target, BTN_SELECT);
    NODE_DEFINE_CONSTANT(target, BTN_START);
    NODE_DEFINE_CONSTANT(target, BTN_MODE);
    NODE_DEFINE_CONSTANT(target, BTN_THUMBL);
    NODE_DEFINE_CONSTANT(target, BTN_THUMBR);

    NODE_DEFINE_CONSTANT(target, BTN_DIGI);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_PEN);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_RUBBER);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_BRUSH);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_PENCIL);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_AIRBRUSH);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_FINGER);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_MOUSE);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_LENS);
    NODE_DEFINE_CONSTANT(target, BTN_TOUCH);
    NODE_DEFINE_CONSTANT(target, BTN_STYLUS);
    NODE_DEFINE_CONSTANT(target, BTN_STYLUS2);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_DOUBLETAP);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_TRIPLETAP);
    NODE_DEFINE_CONSTANT(target, BTN_TOOL_QUADTAP);

    NODE_DEFINE_CONSTANT(target, BTN_WHEEL);
    NODE_DEFINE_CONSTANT(target, BTN_GEAR_DOWN);
    NODE_DEFINE_CONSTANT(target, BTN_GEAR_UP);

}
