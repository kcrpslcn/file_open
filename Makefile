
.PHONY: help
help: ## Show this help
	@grep -E '^[a-zA-Z_-]+:.*?## .*$$' $(MAKEFILE_LIST) | sort | awk 'BEGIN {FS = ":.*?## "}; {printf "\033[36m%-20s\033[0m %s\n", $$1, $$2}'

.PHONY: build
build: ## Build the application
	dart pub get
	cd example && dart pub get
	dart run pigeon --input pigeons/**

.PHONY: clean
clean: ## Clean the build artifacts
	flutter clean
	cd example && flutter clean
	rm -rf coverage

.PHONY: upgrade
upgrade: ## Upgrade dependencies
	dart pub upgrade
	cd example && dart pub upgrade

.PHONY: example_macos
example_macos: ## Run the example_macos application
	cd example && flutter run -d macos

.PHONY: example_windows
example_windows: ## Run the example_windows application
	cd example && flutter run -d windows