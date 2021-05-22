#include <string>

#include <tidy.h>
#include <tidybuffio.h>

#include "error.h"

// https://www.html-tidy.org/developer/
std::string html_tidy(const std::string &html) {
  TidyDoc doc = tidyCreate();
  tidyOptSetBool(doc, TidyXhtmlOut, yes);
  tidyOptSetBool(doc, TidyShowWarnings, no);
  tidyOptSetInt(doc, TidyWrapLen, 0);
  tidyOptSetBool(doc, TidyHideComments, yes);

  auto rc = tidyParseString(doc, html.c_str());

  if (rc >= 0) {
    rc = tidyCleanAndRepair(doc);
  }

  TidyBuffer output = {};
  if (rc >= 0) {
    rc = tidySaveBuffer(doc, &output);
  }

  std::string xhtml;
  if (rc >= 0) {
    xhtml.assign(reinterpret_cast<const char *>(output.bp), output.size);
  } else {
    error("error");
  }

  tidyBufFree(&output);
  tidyRelease(doc);

  return xhtml;
}

int main() {
  EXPECT(html_tidy("<title>Foo</title><p>Foo!") == R"(<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta name="generator" content="HTML Tidy for HTML5 for Linux version 5.7.28" />
<title>Foo</title>
</head>
<body>
<p>Foo!</p>
</body>
</html>
)");
}
