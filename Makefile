build:
		@echo "Nothing to build."

rebuild: clear restart

build-all: clear explorer attack-rtlo reverse-reverse1 restart

# Explorer
explorer:
		@echo "Bringing up explorer..."
		@docker-compose up --no-deps -d explorer

# Attacks
attack-rtlo:
		@echo "Building attack-rtlo..."
		@docker build -t attack-rtlo -f docker/attack/rtlo/Dockerfile .
		@docker-compose up --no-deps -d attack-rtlo

# Reverse
reverse-reverse1:
		@echo "Building reverse-reverse1..."
		@docker build -t reverse-reverse1 -f docker/reverse/reverse-1/Dockerfile .
		@docker-compose up --no-deps -d reverse-reverse1

# Web
web:
		@echo "Building web..."
		@docker-compose up --no-deps -d hackme hackme-php

# Web
breakme:
		@echo "Building breakme..."
		@docker-compose up --no-deps -d breakme-web breakme-php

# Compose
restart:
		@echo "Pulling down docker-compose..."
		@docker-compose down
		@echo "Pulling up docker-compose..."
		@docker-compose up -d

clear:
		@clear