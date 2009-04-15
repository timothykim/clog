#include <stdio.h>
#include <stdlib.h>
#include "fcgi_stdio.h"
#include "clog.h"

int main() {
    while (FCGI_Accept() >= 0) {
		printf("Content-type: text/html\r\n"
				"\r\n"
                
                "<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"> "
                "<html xmlns=\"http://www.w3.org/1999/xhtml\">" 
                "<head>"
                "    <meta name=\"Author\" content=\"Timothy Kim\" />"
                "    <meta http-equiv=\"content-type\" content=\"text/html; charset=utf-8\" />"
                "    <style type=\"text/css\">"
                "        @import url('clog.css');"
                "    </style>"
                "    <title>timothylive.net</title>"
                "</head>"
                "<body>"
                "    <div id=\"body_wrapper\">"
                );
        print_entries(2, 1, 
            "<div class=\"entry\" id=\"entry%s\">"
            "<div class=\"title\">%s</div>"
            "<div class=\"datetime\">%s</div>"
            "<div class=\"content\">%s</div>"
            "</div>"
        );
        printf("</div></body></html>");
    }

	return 0;
}
