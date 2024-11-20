# Use a lightweight base image for the build stage
FROM debian:bullseye-slim AS build

# Set working directory
WORKDIR /app

# RUN echo "nameserver 8.8.8.8" > /etc/resolv.conf && apt-get update

# Install dependencies
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       build-essential \
       cmake \
       libssl-dev \
       libjsoncpp-dev \
    && rm -rf /var/lib/apt/lists/*

# Copy source code
COPY . .

# Build the application
RUN cmake -B build && cmake --build build

# Runtime stage
FROM debian:bullseye-slim

# Install runtime dependencies
RUN apt-get update \
    && apt-get install -y --no-install-recommends \
       libssl1.1 \
       libjsoncpp1 \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy the executable from the build stage
COPY --from=build /app/build/vitaledge-crypt /app/vitaledge-crypt

# Expose the application port
EXPOSE 8084

# Run the application
CMD ["./vitaledge-crypt"]
