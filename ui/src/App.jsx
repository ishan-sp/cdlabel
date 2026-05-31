import React, { useMemo, useRef, useState } from "react";

const DEFAULT_CODE = `#include <stdio.h>

float multiply(float a, float b) {
    return a * b;
}

int main() {
    float a = 1e8f;
    float b = 1.0000001f;
    float c = multiply(a, b);
    printf("%f\\n", c);
    return 0;
}`;

const MOCK_WARNINGS = [
  {
    line: 9,
    message: "Catastrophic cancellation detected",
    detail: "float result 100000008 vs double shadow 100000011.92"
  }
];

export default function App() {
  const [code, setCode] = useState(DEFAULT_CODE);
  const [warnings, setWarnings] = useState([]);
  const [output, setOutput] = useState("");
  const [status, setStatus] = useState("Idle");
  const [isRunning, setIsRunning] = useState(false);

  const textareaRef = useRef(null);
  const linesRef = useRef(null);

  const lineCount = useMemo(() => code.split("\n").length, [code]);
  const lineNumbers = useMemo(() => {
    return Array.from({ length: lineCount }, (_, i) => i + 1);
  }, [lineCount]);

  const syncScroll = () => {
    if (!textareaRef.current || !linesRef.current) return;
    linesRef.current.scrollTop = textareaRef.current.scrollTop;
  };

  const lineToIndex = (line) => {
    const lines = code.split("\n");
    let index = 0;
    for (let i = 0; i < line - 1 && i < lines.length; i += 1) {
      index += lines[i].length + 1;
    }
    return index;
  };

  const focusLine = (line) => {
    if (!textareaRef.current) return;
    const index = lineToIndex(line);
    textareaRef.current.focus();
    textareaRef.current.setSelectionRange(index, index);
    const lineHeight = 22;
    textareaRef.current.scrollTop = Math.max(0, (line - 1) * lineHeight - 44);
    syncScroll();
  };

  const runCompile = async () => {
    setIsRunning(true);
    setStatus("Compiling...");
    setWarnings([]);
    setOutput("");

    try {
      const response = await fetch("/api/numsan", {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ code })
      });

      if (!response.ok) {
        throw new Error("Backend not available");
      }

      const data = await response.json();
      setWarnings(Array.isArray(data.warnings) ? data.warnings : []);
      setOutput(data.output || "");
      setStatus("Complete");
    } catch (error) {
      setWarnings(MOCK_WARNINGS);
      setOutput("[demo] No backend connected. Showing sample output.");
      setStatus("Demo mode");
    } finally {
      setIsRunning(false);
    }
  };

  return (
    <div className="page">
      <header className="hero">
        <div>
          <p className="eyebrow">Numerical Sanitizer</p>
          <h1>NumSan Studio</h1>
          <p className="subtitle">
            Type C code, compile, and inspect precision warnings with line-level
            context.
          </p>
        </div>
        <div className="actions">
          <button className="primary" onClick={runCompile} disabled={isRunning}>
            {isRunning ? "Compiling..." : "Compile"}
          </button>
          <span className="status">{status}</span>
        </div>
      </header>

      <main className="grid">
        <section className="panel editor">
          <div className="panel-header">
            <h2>Editor</h2>
            <span>{lineCount} lines</span>
          </div>
          <div className="editor-body">
            <div className="line-numbers" ref={linesRef} aria-hidden="true">
              {lineNumbers.map((line) => (
                <div key={line}>{line}</div>
              ))}
            </div>
            <textarea
              ref={textareaRef}
              value={code}
              onChange={(event) => setCode(event.target.value)}
              onScroll={syncScroll}
              spellCheck={false}
            />
          </div>
        </section>

        <section className="panel warnings">
          <div className="panel-header">
            <h2>Warnings</h2>
            <span>{warnings.length}</span>
          </div>
          <div className="panel-body">
            {warnings.length === 0 ? (
              <p className="empty">No warnings yet.</p>
            ) : (
              warnings.map((warning, index) => (
                <button
                  key={`${warning.line}-${index}`}
                  className="warning-item"
                  onClick={() => focusLine(warning.line)}
                >
                  <span className="badge">Line {warning.line}</span>
                  <div>
                    <h3>{warning.message}</h3>
                    <p>{warning.detail}</p>
                  </div>
                </button>
              ))
            )}
          </div>
        </section>

        <section className="panel output">
          <div className="panel-header">
            <h2>Output</h2>
          </div>
          <pre className="panel-body output-body">{output || "No output yet."}</pre>
        </section>
      </main>
    </div>
  );
}
