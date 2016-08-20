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

/* Return priority of match/section */
int route_priority(char * str) {
	if (!str) {
		return 0;
	}
	else if (str[0] == '*' && str[1] == '\0') {
		/* Wildcard */
		return 3;
	}
	else if (str[0] == '@' && str[1] == '\0') {
		/* Parameter */
		return 2;
	}
	else {
		/* Constant */
		return 1;
	}
}

void route_add(Route *root, const char *path, Handler handler, int methods) {
	const char *cursor = path, *cursor_next;
	char *section;
	Route *pRoute = root;
	RouteChild *pChild, *pTemp;
	int len, action, priority;

	printf("route_add (%s):\n", path);
	while (*cursor) {
		/* Skip slash. */
		if (*cursor == '/') {
			cursor++;
		}
		/* Find start of next section (cursor_next). */
		cursor_next = cursor;
		for (; *cursor_next && *cursor_next != '/'; cursor_next++);
		len = cursor_next - cursor;

		if (len == 1 && cursor[0] == '*' && cursor[1] != '\0') {
			puts("WARNING: Wildcard not at end of route.");
		}

		/* +1 for \0 marker. */
		section = malloc((len + 1) * sizeof(char));
		section[len] = '\0';
		memcpy(section, cursor, len * sizeof(char));
		
		priority = route_priority(section);
		printf("Section: %s -  Pri: %i\n", section, priority);

		pChild = pRoute->pChildren;


		/* Check head first, may need to insert before head. */
		action = (pChild && priority < route_priority(pChild->pRoute->match)) ? 1 : 0;

		if (pChild) {
			printf("Head: %s, Pri: %i\n", pChild->pRoute->match, route_priority(pChild->pRoute->match));
		}

		/* Search for match or last item at desired priority order. */
		while (!action && pChild) {
			if (priority == route_priority(pChild->pRoute->match) && strcmp(section, pChild->pRoute->match) == 0) {
				/* Matched */
				action = 2;
			}
			else if (!pChild->pNext) {
				/* End of list reached */
				action = 3;
			}
			else if (priority < route_priority(pChild->pNext->pRoute->match)) {
				/* Next item has higher priority, insert after current. */
				action = 3;
			}
			else {
				/* Move to next */
				pChild = pChild->pNext;
			}
		}

		switch (action) {
			case 0:
				/* Empty list. */
				puts("route_add: Action - Insert into empty list.");
				pChild = malloc(sizeof(RouteChild));
				pRoute->pChildren = pChild;
				pChild->pNext = NULL;
				pRoute = malloc(sizeof(Route));
				pChild->pRoute = pRoute;
				pRoute->match = section;
				pRoute->pChildren = NULL;
				pRoute->handler = NULL;
				break;
			case 1:
				/* Insert at head - non-empty. */
				puts("route_add: Action - Insert at head.");
				pChild = malloc(sizeof(RouteChild));
				pChild->pNext = pRoute->pChildren;
				pRoute->pChildren = pChild;
				pRoute = malloc(sizeof(Route));
				pChild->pRoute = pRoute;
				pRoute->match = section;
				pRoute->pChildren = NULL;
				pRoute->handler = NULL;
				break;
			case 2:
				/* Matched */
				puts("route_add: Action - Matched.");
				pRoute = pChild->pRoute;
				free(section);
				break;
			case 3:
				/* Insert after. */
				puts("route_add: Action - Insert after.");
				pTemp =  malloc(sizeof(RouteChild));
				pTemp->pNext = pChild->pNext;
				pChild->pNext = pTemp;
				pRoute = malloc(sizeof(Route));
				pTemp->pRoute = pRoute;
				pRoute->match = section;
				pRoute->pChildren = NULL;
				pRoute->handler = NULL;
				break;
			default:
				break;
		}
		cursor = cursor_next;
	}
	pRoute->handler = handler;
	pRoute->methods = methods;
}