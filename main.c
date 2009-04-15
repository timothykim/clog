#include <stdio.h>
#include "clog.h"

int main() {
    print_entries(2, 1, 
            "<div class=\"entry\" id=\"entry%s\">"
            "<div class=\"title\">%s</div>"
            "<div class=\"datetime\">%s</div>"
            "<div class=\"content\">%s</div>"
            "</div>"
        );

	return 0;
}
