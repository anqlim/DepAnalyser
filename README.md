# DepAnalyser

A tool for visualizing and analyzing dependency graphs of software projects.
Supports C++, Python, Java, JavaScript/TypeScript, and Go.

## What it does

- Parses source files and builds a directed dependency graph
- Detects **cycles** via Tarjan's SCC algorithm
- Identifies **critical vertices** via Brandes betweenness centrality
- Finds **redundant (transitive) dependencies** via transitive reduction
- Visualizes the graph interactively in the browser

## Architecture

- **C++ backend** — graph construction, multithreaded parsing, algorithm execution
- **Redis** — caches analysis results by content hash; repeated requests are instant
- **D3.js frontend** — force-directed interactive graph visualization
- **Docker** — fully containerized, runs with a single command

## Getting started

**Requirements:** Docker Desktop

```bash
git clone https://github.com/your-username/DepAnalyser
cd DepAnalyser
docker compose up --build
```

Open `http://localhost:8080`, paste a GitHub repo URL or upload a ZIP archive.

## Tech stack

C++20 · Crow · Redis · D3.js · Docker