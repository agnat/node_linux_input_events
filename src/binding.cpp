#include <v8.h>
#include <linux/input.h>

#include <iostream>

#include <node_buffer.h>
#include <node_events.h>

using namespace v8;
using namespace node;

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
            std::cout << "DispatchEventsInBuffer" << std::endl;
            HandleScope scope;
            if (args.Length() != 1) {
                return ThrowException(Exception::Error(String::New("expected 1 argument")));
            }
            if (! Buffer::HasInstance(args[0])) {
                return ThrowException(Exception::Error(String::New("argument must be a buffer")));
            }
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
                Local<Object> jsevent = Object::New();
                Local<Value> args[1];
                args[0] = jsevent;
                Emit(input_event_symbol, 1, args);
            }
        }
        
        static Persistent<String> input_event_symbol;

        static Persistent<String> time_symbol;
        static Persistent<String> type_symbol;
        static Persistent<String> code_symbol;
        static Persistent<String> value_symbol;
};

Persistent<String> Source::input_event_symbol;

Persistent<String> Source::time_symbol;
Persistent<String> Source::type_symbol;
Persistent<String> Source::code_symbol;
Persistent<String> Source::value_symbol;

extern "C"
void
init(Handle<Object> target) {
    HandleScope scope;
//    target->Set(String::NewSymbol("parse_input_event"), FunctionTemplate::New(parse_input_event)->GetFunction());
    Source::init(target);
}
