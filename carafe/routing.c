
Route * routes_head;
Route * routes_tail;

Handler route_request(Request * req) {
	Route * cur = routes_head;
	while (cur != NULL) {
		/* some routing logic */
		cur =  cur->next;
	}
	return NULL;
}

void route_add(char * p, Handler * h) {
	Route * r = malloc(sizeof(Route));
	r->pattern = p;
	r->handler = h;
	r->next = NULL;
	if (routes_head == NULL) {
		routes_head = r;
		routes_tail = r;
	}
	else {
		routes_tail->next = r;
		routes_tail = r;
	}
}