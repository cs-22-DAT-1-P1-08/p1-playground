#include <stdio.h>
#include <json-c/json.h>

int main() {
    // Author: Thibault Guittet (https://alan-mushi.github.io/)
    // https://gist.github.com/alan-mushi/19546a0e2c6bd4e059fd
    struct json_object *jobj;
    char *str = "{ \"msg-type\": [ \"0xdeadbeef\", \"irc log\" ], \
		\"msg-from\": { \"class\": \"soldier\", \"name\": \"Wixilav\" }, \
		\"msg-to\": { \"class\": \"supreme-commander\", \"name\": \"[Redacted]\" }, \
		\"msg-log\": [ \
			\"soldier: Boss there is a slight problem with the piece offering to humans\", \
			\"supreme-commander: Explain yourself soldier!\", \
			\"soldier: Well they don't seem to move anymore...\", \
			\"supreme-commander: Oh snap, I came here to see them twerk!\" \
			] \
		}";

    printf("str:\n---\n%s\n---\n\n", str);

    jobj = json_tokener_parse(str);
    json_object *msglog = json_object_object_get(jobj, "msg-log");
    printf("jobj from str:\n---\n%s\n---\n", json_object_get_string(msglog));

    return 0;
}
