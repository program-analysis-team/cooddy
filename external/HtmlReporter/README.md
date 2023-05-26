# HTML reporter
This is a tool for representing source code analysis results in html format.

To generate an HTML report, specify a source report in Static Analysis Results Interchange Format (SARIF) or Cooddy json/html. Additionally you need to specify a path to the source files used for analysis, so that code traces for found issues may be displayed correctly.

# Command line options
| Option         | Description                                                       |
|----------------|-------------------------------------------------------------------|
| --report       | Specify file or folder with json/html/sarif with list of problems |
| --sources-path | Path to the analyzed project source code root folder              |
| --result       | Output report file or directory. Default "./cooddy_result.html"   |
