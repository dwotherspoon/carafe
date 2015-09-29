#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <lib/router.h>

#ifdef TESTS
	#include <stdio.h>
#else
	#include <fcgi_stdio.h>
#endif


int parse_method(char * method) {
	/* Be fast, only check first three chars. */
	switch ((method[0] << 0x10) | (method[1] << 0x08) | (method[2])) {
		case 0x474554:
			return GET;
			break;
		case 0x504F53:
			return POST;
			break;
		case 0x505554:
			return PUT;
			break;
		case 0x504154:
			return PATCH;
			break;
		case 0x44454C:
			return DELETE;
			break;
		case 0x434F50:
			return COPY;
			break;
		case 0x484541:
			return HEAD;
			break;
		case 0x4F5054:
			return OPTIONS;
			break;
		case 0x4C494E:
			return LINK;
			break;
		case 0x554E4C:
			return UNLINK;
			break;
		case 0x505552:
			return PURGE;
			break;
		default:
			return 0;
			break;
	}
}

void route_init(Route *root) {
	root->match = NULL;
	root->methods = 0xFFFFFFFF;
	root->pChildren = NULL;
}

static int route_matchcmp(char * value, char * match, int value_len) {
	return 0;
}

Handler route_request(Route *root, Request *req) {
	const char *cursor = (char *)hashtable_get(&(req->vars), "REQUEST_URI", sizeof("REQUEST_URI"));
	const char *cursor_next;
	int len, found;
	Route *pRoute = root;
	RouteChild *pChild;

	while (*cursor) {
		if (*cursor == '/') {
			cursor++;
		}
		/* Find start of next section */
		cursor_next = cursor;
		for (; *cursor_next && *cursor_next != '/'; cursor_next++);
		len = cursor_next - cursor;

		pChild = pRoute->pChildren;
		found = 0;

		while (!found && pChild) {
			/* Make sure to match EOS */
			if (!strncmp(cursor, pChild->pRoute->match, len + 1) || *(pChild->pRoute->match) == '*') {
				found = 1;
			}
			else {
				pChild = pChild->pNext;
			}
		}

		if (!found) {
			return NULL;
		}
		else {
			pRoute = pChild->pRoute;
			printf("%s matched to %s\r\n", cursor, pRoute->match);
		}

		cursor = cursor_next;
	}
	return pRoute->handler;
}


void route_debug(Route *root, int level) {
	RouteChild *pChild = root->pChildren;
	if (root->match) {
		if (root->handler) {
			printf("%*s/%s => %p\n", level << 1, "", root->match, root->handler);
		}
		else {
			printf("%*s/%s\n", level << 1, "", root->match);
		}
	}
	while (pChild) {
		route_debug(pChild->pRoute, level + 1);
		pChild = pChild->pNext;
	}
}

/* TODO: Maintain priority queue, constant has highest priority, @ param next and then wildcard. */
void route_add(Route *root, const char *path, Handler handler, int methods) {
	const char *cursor = path, *cursor_next;
	char *section;
	Route *pRoute = root;
	RouteChild *pChild;
	int action, len;

	puts("route_add");
	while (*cursor) {
		if (*cursor == '/') {
			cursor++;
		}
		/* Find start of next section */
		cursor_next = cursor;
		for (; *cursor_next && *cursor_next != '/'; cursor_next++);
		len = cursor_next - cursor;
		/* +1 for EOS marker */
		section = malloc((len + 1) * sizeof(char));
		section[len] = '\0';
		memcpy(section, cursor, len * sizeof(char));
		printf("Section: %s(len %d)\n", section, len);

		pChild = pRoute->pChildren;
		action = 0;
		/* 0 = search/empty, 1 = found, 2 = not found */
		while (!action && pChild) {
			/* Check this doesn't break for prefixes */
			if (!strncmp(pChild->pRoute->match, section, len + 1)) {
				action = 1;
			}
			else if (!pChild->pNext) {
				/* No next child, stop. */
				action = 2;
			}
			else {
				pChild = pChild->pNext;
			}
		}

		if (action == 1) {
			puts("Found match...");
			/* Free the section string, we don't need it. */
			free(section);
			/* Follow node */
			pRoute = pChild->pRoute;
		}
		else {
			puts("Adding new child...");
			if (!action) {
				puts("List is empty");
				/* Empty list */
				pRoute->pChildren = malloc(sizeof(RouteChild));
				pChild = pRoute->pChildren;
			}
			else {
				/* Non-empty list */
				pChild->pNext = malloc(sizeof(RouteChild));
				pChild = pChild->pNext;
			}
			pChild->pNext = NULL;
			pChild->pRoute = malloc(sizeof(Route));
			pRoute = pChild->pRoute;
			pRoute->pChildren = NULL;
			pRoute->match = section;
		}
		cursor = cursor_next;
	}
	/* Add handler to final node. */
	pRoute->handler = handler;
	pRoute->methods = methods;
}
