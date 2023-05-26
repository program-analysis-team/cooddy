## Build cooddy

Check following page on [how-to build Cooddy.](Build.md)

## Sample checker project hierarchy

```
extensibility
├─include					// cooddy includes directory 
└─sample_checker
  ├─CMakeLists.txt
  ├─Checkers.cpp			// declaration of checkers, if you want to create a new checker, you have to declare a new checker with DECLARE_CHECKER(<CheckerClassName>) macro
  ├─sample.profile.json		// checkers profile
  ├─SampleAstChecker.cpp 	// sample of AST-based checker
  └─SampleDfaChecker.cpp	// sample of DFA-based checker
```

## Modify checker's profile

You can modify the sample.profile.json file.

```json
{
  "name": "My custom checkers",
  "module": "libSampleChecker",
  "languages": [
    "c/c++",
    "java"
  ],
  "checkers": {
    "SampleDfaChecker": {
      "description": "This problem has been found by my DFA checker",
      "languages": [
        "c/c++"
      ],
      "id": "Sample",
      "severity": "Error",
      "enabled": true
    },
    "SampleAstChecker": {
      "description": "This problem has been found by my AST checker",
      "languages": [
        "c/c++"
      ],
      "id": "Sample",
      "severity": "Warning",
      "enabled": true
    }
  }
}
```

Where:

* name: the name of the profile is used in report files json/csv to categorize detected defects
* module: library name created on the previous step
* languages: a set of programming languages that checker can analyze. Inspection can override the checker languages. And checker can inherit languages from profile. Acceptable values are "c/c++" and "java". Default value is "c/c++". In example above MyChecker overrides profile languages.
* description: the description of the problem, which is reported by your checker
* id: any unique id
* severity: severity of the reported problem (Error, Warning, Info)
* enable: if your checker is enabled by default

## Run your checker

You can use the following command to run cooddy with your checker:

```bash
./cooddy --scope=с:\test\test.c --profile=sample
```
