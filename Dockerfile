# ─── Stage 1: сборка ───────────────────────────────────────────────────────
FROM ubuntu:24.04 AS builder

# Без этого apt лезет за временной зоной и зависает
ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    g++ \
    git \
    libhiredis-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копируем всё и собираем
COPY . .
RUN cmake -B build -DCMAKE_BUILD_TYPE=Release && cmake --build build -j$(nproc)

# ─── Stage 2: запуск ───────────────────────────────────────────────────────
FROM ubuntu:22.04 AS runtime

ENV DEBIAN_FRONTEND=noninteractive

# Только то что нужно для запуска — без компилятора
RUN apt-get update && apt-get install -y \
    libhiredis0.14 \
    git \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Копируем только бинарник из stage 1
COPY --from=builder /app/build/DepAnalyser .

EXPOSE 8080

CMD ["./DepAnalyser"]