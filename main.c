#include <stdio.h>
#include "clog.h"

int main() {
    print_entries(2, 1, 
            "<div class=\"entry\" id=\"entry{id}\">"
            "<div class=\"title\">{title}</div>"
            "<div class=\"datetime\">{c_time}</div>"
            "<div class=\"content\">{content}</div>"
            "<div class=\"update\">{u_time}</div>"
            "</div>"
        );

	return 0;
}
