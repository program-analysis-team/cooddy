{
  "language": "Any",
  "standard": "Any",
  "version": "Last",
  "languages" : ["c", "c++", "java"],

  "annotations": [".annotations-test.json"],
  "inspections": {
    "GenericTaintChecker": {
      "id": "GenericTaintChecker",
      "description": "Generic taint checker",
      "severity": "Error",
      "enabled": true,
      "settings": {
        "config": "TaintConfig"
      }
    },
    "FormatStringUsageChecker": {
      "id": "CWE-20",
      "description": "Format parameter is externally controllable.",
      "severity": "Error",
      "enabled": true,
      "settings": {
        "config": ""
      }
    },
    "UninitializedMemoryUsageChecker": {
      "id": "CWE-824",
      "description": "The variable '' is an uninitialized value.",
      "severity": "Error",
      "enabled": true,
      "settings": {
        "config": "SSR_5.3"
      }
    },
    "NullPtrDereferenceChecker": {
      "id": "CWE-476",
      "description": "Null dereference",
      "severity": "Error",
      "enabled": true,
      "settings": {
        "config": ""
      }
    },
    "UseAfterFreeChecker": {
      "id": "CWE-416",
      "description": "Use after free",
      "severity": "Error",
      "enabled": true,
      "settings": {
        "config": ""
      }
    },
    "DivisionByZeroChecker": {
      "id": "CWE-190",
      "description": "Division by zero",
      "severity": "Error",
      "enabled": true,
      "settings": {
        "config": ""
      }
    }
  }
}
