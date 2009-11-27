#include <libxml/xmlreader.h>
#include <string.h>
#include "atom.h"
#include "hash.h"

#ifdef LIBXML_READER_ENABLED

int parse_post(const char *xml, char *title, int title_size, char *content, int content_size) {
    xmlTextReaderPtr reader;
    int ret;

    reader = xmlReaderForDoc(xml, NULL, NULL, 0);

    if (reader == NULL) {
        return -1;
    }

    ret = xmlTextReaderRead(reader);
    while (ret == 1) {
        if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT
                && strcmp(xmlTextReaderConstName(reader), "title") == 0) {
            ret = xmlTextReaderRead(reader);
            strncpy(title, xmlTextReaderConstValue(reader), title_size);
        } else
        if (xmlTextReaderNodeType(reader) == XML_READER_TYPE_ELEMENT
                && strcmp(xmlTextReaderConstName(reader), "content") == 0) {
            ret = xmlTextReaderRead(reader);
            strncpy(content, xmlTextReaderConstValue(reader), content_size);
        }
        ret = xmlTextReaderRead(reader);
    }

    xmlFreeTextReader(reader);

    if (ret != 0) {
        return -1;
    }
    return 0;
}

#endif
