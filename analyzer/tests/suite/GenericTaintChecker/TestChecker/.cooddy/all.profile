{
  "language": "Any",
  "standard": "Any",
  "version": "Last",
  "annotations": [],
  "checkers": {
    "GenericTaintChecker": {
      "inspections" : [
        {"id": "MyChecker", "severity": "Error", "enabled": true, "description": "This parameter is externally controllable."}
      ],
      "settings": { "config": "TaintConfig" }
    }
  }
}
