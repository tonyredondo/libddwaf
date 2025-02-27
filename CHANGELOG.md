# libddwaf release

### v1.6.2  ([unstable](https://github.com/DataDog/libddwaf/blob/master/README.md#versioning-semantics)) - 2023/01/26
#### Changes
- Add boolean getter ([#132](https://github.com/DataDog/libddwaf/pull/132))
- Add support for converting string to bool in parameter bool cast operator ([#133](https://github.com/DataDog/libddwaf/pull/133))
- Add parameter `int64_t` cast operator ([#133](https://github.com/DataDog/libddwaf/pull/133))
- Add support for `enabled` flag on ruleset parser ([#133](https://github.com/DataDog/libddwaf/pull/133))

#### Fixes
- Replace `isdigit` with custom version due to windows locale-dependence ([#133](https://github.com/DataDog/libddwaf/pull/133))
- Minor fixes and parsing improvements ([#133](https://github.com/DataDog/libddwaf/pull/133))

### v1.6.1 ([unstable](https://github.com/DataDog/libddwaf/blob/master/README.md#versioning-semantics)) - 2023/01/17

#### Miscellaneous
- Add SHA256 to packages ([#128](https://github.com/DataDog/libddwaf/pull/128))
- Automatic draft release on tag ([#129](https://github.com/DataDog/libddwaf/pull/129))

### v1.6.0 ([unstable](https://github.com/DataDog/libddwaf/blob/master/README.md#versioning-semantics)) - 2023/01/10

#### Changes
- Exclusion filters: targets and conditions ([#110](https://github.com/DataDog/libddwaf/pull/110))
- Exclusion filters: inputs ([#117](https://github.com/DataDog/libddwaf/pull/117))
- Add ID to exclusion filters ([#120](https://github.com/DataDog/libddwaf/pull/120))
- Rework path trie for exclusion ([#122](https://github.com/DataDog/libddwaf/pull/122))
- Priority collections ([#123](https://github.com/DataDog/libddwaf/pull/123))
- Support for glob component and arrays on object filter ([#124](https://github.com/DataDog/libddwaf/pull/124))

#### Miscellaneous
- Experiment building libddwaf on the oldest available macos target ([#111](https://github.com/DataDog/libddwaf/pull/111))
- Strip libddwaf.a for darwin/linux release ([#107](https://github.com/DataDog/libddwaf/pull/107))
- linux/aarch64: add missing libunwind.a artefact ([#109](https://github.com/DataDog/libddwaf/pull/109))
- Add option to prevent loading test targets ([#108](https://github.com/DataDog/libddwaf/pull/108))
- Upgrade deprecated actions ([#114](https://github.com/DataDog/libddwaf/pull/114))
- Include mac arm binaries in nuget ([#115](https://github.com/DataDog/libddwaf/pull/115))
- Run clang tidy / format on CI ([#116](https://github.com/DataDog/libddwaf/pull/116))
- Exclusion filters on fuzzer ([#118](https://github.com/DataDog/libddwaf/pull/118))

### v1.5.1 ([unstable](https://github.com/DataDog/libddwaf/blob/master/README.md#versioning-semantics)) - 2022/09/22

#### API \& Breaking Changes
- Add `ddwaf_required_rule_data_ids` to obtain the rule data IDs defined in the ruleset ([#104](https://github.com/DataDog/libddwaf/pull/104))

#### Miscellaneous
- GTest `ddwaf_result` validators ([#102](https://github.com/DataDog/libddwaf/pull/102))
- Replace `std::optional::value()` with `std::optional::operator*()` ([#105](https://github.com/DataDog/libddwaf/pull/105))
- Add new and missing exports ([#106](https://github.com/DataDog/libddwaf/pull/106))

### v1.5.0 ([unstable](https://github.com/DataDog/libddwaf/blob/master/README.md#versioning-semantics)) - 2022/09/08

#### API \& Breaking Changes
- Remove `ddwaf_version`, `ddwaf_get_version` now returns a version string ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Move free function from `ddwaf_context_init` to `ddwaf_config` ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Add `ddwaf_result.actions` struct containing a `char*` array and its size ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Add `ddwaf_update_rule_data` ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Remote `DDWAF_BLOCK` ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Rename `DDWAF_GOOD` to `DDWAF_OK` ([#92](https://github.com/DataDog/libddwaf/pull/92))
- Rename `DDWAF_MONITOR` to `DDWAF_MATCH` ([#92](https://github.com/DataDog/libddwaf/pull/92))
- Deanonymize nested structs ([#97](https://github.com/DataDog/libddwaf/pull/97))
- Add `ddwaf_object_bool` for backwards-compatible support for boolean `ddwaf_object` ([#99](https://github.com/DataDog/libddwaf/pull/99))
- Add `ddwaf_toggle_rules` to enable or disable rules at runtime ([#99](https://github.com/DataDog/libddwaf/pull/99))

#### Changes
- Add `unicode_normalize` transformer ([#82](https://github.com/DataDog/libddwaf/pull/82))
- Remove `PWRetriever`, `PWArgsWrapper`, `Iterator` and `ArgsIterator` ([#77](https://github.com/DataDog/libddwaf/pull/77))
- Add `ddwaf::object_store` to manage all targets and objects provided to the WAF ([#77](https://github.com/DataDog/libddwaf/pull/77))
- Add `ddwaf::value_iterator` for object value traversal ([#77](https://github.com/DataDog/libddwaf/pull/77))
- Add `ddwaf::key_iterator` for object key traversal ([#77](https://github.com/DataDog/libddwaf/pull/77))
- Simplify target manifest ([#78](https://github.com/DataDog/libddwaf/pull/78))
- Remove input object validation ([#85](https://github.com/DataDog/libddwaf/pull/85))
- Merge `PWAdditive` and `PWProcessor`and rename to `ddwaf::context` ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Rename `PowerWAF` to `ddwaf::waf` ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Add `ddwaf::timer` to abstract deadline ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Simplify rule processors ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Add `ip_match` operator and tests ([#87](https://github.com/DataDog/libddwaf/pull/87))
- Refactor ip handling into `ip_utils` ([#87](https://github.com/DataDog/libddwaf/pull/87))
- Add `exact_match` operator and tests ([#87](https://github.com/DataDog/libddwaf/pull/87))
- Rename existing rule processors to more closely resemble their operator name ([#87](https://github.com/DataDog/libddwaf/pull/87))
- Rename `IPWRuleProcessor` to `rule_processor_base` ([#87](https://github.com/DataDog/libddwaf/pull/87))
- Add support for per-rule `on_match` array in ruleset ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Add optional `on_match` to JSON event format ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Remove `PWRetManager` and `MatchGatherer` ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Add `ddwaf::event` to collect all relevant rule match data in one structure ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Add `ddwaf::event_serializer` for JSON event ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Update processors to use `std::string_view` rather than `char *` and length ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Add `ddwaf::timeout_exception` to avoid error code propagation ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Disable the `1)c` libinjection fingerprint ([#94](https://github.com/DataDog/libddwaf/pull/94))
- Configurable rule data ([#96](https://github.com/DataDog/libddwaf/pull/96))

#### Fixes
- Timeout error propagation ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Pass object limits configuration to iterators ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Apply string limits ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Fix missing exports ([#101](https://github.com/DataDog/libddwaf/pull/101))

#### Miscellaneous
- Add `utf8proc` license ([#84](https://github.com/DataDog/libddwaf/pull/84))
- Add codecov support ([#86](https://github.com/DataDog/libddwaf/pull/86))
- Add CODEOWNERS  ([#88](https://github.com/DataDog/libddwaf/pull/88))
- Add `benchmerge` to merge multiple benchmark results ([#85](https://github.com/DataDog/libddwaf/pull/85))
- Update ruleset version for testing to 1.3.2 ([#101](https://github.com/DataDog/libddwaf/pull/101))
- Fix missing build flags from `utf8proc` build ([#100](https://github.com/DataDog/libddwaf/pull/100))

### v1.5.0-rc0 ([unstable](https://github.com/DataDog/libddwaf/blob/master/README.md#versioning-semantics)) - 2022/09/02

#### API \& Breaking Changes
- Add `ddwaf_object_bool` for backwards-compatible support for boolean `ddwaf_object` ([#99](https://github.com/DataDog/libddwaf/pull/99))
- Add `ddwaf_toggle_rules` to enable or disable rules at runtime ([#99](https://github.com/DataDog/libddwaf/pull/99))

#### Fixes
- Fix missing exports ([#101](https://github.com/DataDog/libddwaf/pull/101))

#### Miscellaneous
- Update ruleset version for testing to 1.3.2 ([#101](https://github.com/DataDog/libddwaf/pull/101))
- Fix missing build flags from `utf8proc` build ([#100](https://github.com/DataDog/libddwaf/pull/100))

### v1.5.0-alpha1 ([unstable](https://github.com/DataDog/libddwaf/blob/master/README.md#versioning-semantics)) - 2022/08/30

#### API \& Breaking Changes
- Deanonymize nested structs ([#97](https://github.com/DataDog/libddwaf/pull/97))

#### Changes
- Disable the `1)c` libinjection fingerprint ([#94](https://github.com/DataDog/libddwaf/pull/94))
- Configurable rule data ([#96](https://github.com/DataDog/libddwaf/pull/96))

### v1.5.0-alpha0 ([unstable](https://github.com/DataDog/libddwaf/blob/master/README.md#versioning-semantics)) - 2022/08/04

#### API \& Breaking Changes
- Remove `ddwaf_version`, `ddwaf_get_version` now returns a version string ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Move free function from `ddwaf_context_init` to `ddwaf_config` ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Add `ddwaf_result.actions` struct containing a `char*` array and its size ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Add dummy `ddwaf_update_rule_data` for future use ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Remote `DDWAF_BLOCK` ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Rename `DDWAF_GOOD` to `DDWAF_OK` ([#92](https://github.com/DataDog/libddwaf/pull/92))
- Rename `DDWAF_MONITOR` to `DDWAF_MATCH` ([#92](https://github.com/DataDog/libddwaf/pull/92))

#### Changes
- Add `unicode_normalize` transformer ([#82](https://github.com/DataDog/libddwaf/pull/82))
- Remove `PWRetriever`, `PWArgsWrapper`, `Iterator` and `ArgsIterator` ([#77](https://github.com/DataDog/libddwaf/pull/77))
- Add `ddwaf::object_store` to manage all targets and objects provided to the WAF ([#77](https://github.com/DataDog/libddwaf/pull/77))
- Add `ddwaf::value_iterator` for object value traversal ([#77](https://github.com/DataDog/libddwaf/pull/77))
- Add `ddwaf::key_iterator` for object key traversal ([#77](https://github.com/DataDog/libddwaf/pull/77))
- Simplify target manifest ([#78](https://github.com/DataDog/libddwaf/pull/78))
- Remove input object validation ([#85](https://github.com/DataDog/libddwaf/pull/85))
- Merge `PWAdditive` and `PWProcessor`and rename to `ddwaf::context` ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Rename `PowerWAF` to `ddwaf::waf` ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Add `ddwaf::timer` to abstract deadline ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Simplify rule processors ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Add `ip_match` operator and tests ([#87](https://github.com/DataDog/libddwaf/pull/87))
- Refactor ip handling into `ip_utils` ([#87](https://github.com/DataDog/libddwaf/pull/87))
- Add `exact_match` operator and tests ([#87](https://github.com/DataDog/libddwaf/pull/87))
- Rename existing rule processors to more closely resemble their operator name ([#87](https://github.com/DataDog/libddwaf/pull/87))
- Rename `IPWRuleProcessor` to `rule_processor_base` ([#87](https://github.com/DataDog/libddwaf/pull/87))
- Add support for per-rule `on_match` array in ruleset ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Add optional `on_match` to JSON event format ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Remove `PWRetManager` and `MatchGatherer` ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Add `ddwaf::event` to collect all relevant rule match data in one structure ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Add `ddwaf::event_serializer` for JSON event ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Update processors to use `std::string_view` rather than `char *` and length ([#91](https://github.com/DataDog/libddwaf/pull/91))
- Add `ddwaf::timeout_exception` to avoid error code propagation ([#91](https://github.com/DataDog/libddwaf/pull/91))

#### Fixes
- Timeout error propagation ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Pass object limits configuration to iterators ([#89](https://github.com/DataDog/libddwaf/pull/89))
- Apply string limits ([#89](https://github.com/DataDog/libddwaf/pull/89))

#### Miscellaneous
- Add `utf8proc` license ([#84](https://github.com/DataDog/libddwaf/pull/84))
- Add codecov support ([#86](https://github.com/DataDog/libddwaf/pull/86))
- Add CODEOWNERS  ([#88](https://github.com/DataDog/libddwaf/pull/88))
- Add `benchmerge` to merge multiple benchmark results ([#85](https://github.com/DataDog/libddwaf/pull/85))

### v1.4.0 ([unstable](https://github.com/DataDog/libddwaf/blob/master/README.md#versioning-semantics)) - 2022/06/29
- Correct nuget url ([#68](https://github.com/DataDog/libddwaf/pull/68))
- Only take params ownership when needed ([#69](https://github.com/DataDog/libddwaf/pull/69))
- WAF Benchmark Utility ([#70](https://github.com/DataDog/libddwaf/pull/70))
- WAF Validator ([#74](https://github.com/DataDog/libddwaf/pull/74))
- Make libinjection look for backticks ([#80](https://github.com/DataDog/libddwaf/pull/80))
- Add version semantic and unstable release information  ([#81](https://github.com/DataDog/libddwaf/pull/81))

### v1.3.0 (unstable) - 2022/04/04
- WAF event obfuscator.
- Add obfuscator configuration to `ddwaf_config`.
- Changes to limits in `ddwaf_config`:
  - Rename `maxArrayLength` to `limits.max_container_size`.
  - Rename `maxMapDepth` to `limits.max_container_depth`.
  - Add `limits.max_string_length`, currently unused.
  - All limits are now `uint32`.
  - Relevant macros renamed accordingly.

### v1.2.1 (unstable) - 2022/03/17
- Fix issue on ruleset error map reallocation causing cached pointer invalidation.
- Add check for empty input map on parser.
- Update github actions windows build VM to windows-2019.

### v1.2.0 (unstable) - 2022/03/16
- Remove metrics collector.
- Add `total_runtime` to `ddwaf_result`.
- Fix issue when reporting timeouts.

### v1.1.0 (unstable) - 2022/03/09
- Add `ddwaf_object` getters.
- Provide ruleset parsing diagnostics on `ddwaf_init`.
- Add support for metrics collection on `ddwaf_run`.
- Add `keys_only` transformer.
- Improve support for older platforms.
- Remove indirection and reduce string operations when processing flows.
- Refactor input verification.
- Remove deprecated features.

### v1.0.18 (unstable) - 2022/02/16
- Add arm64 build to nuget package.
- Upgrade RE2 to 2022-02-01.

### v1.0.17 (unstable) - 2022/01/24
- Add missing libunwind to x86\_64 linux build.
- Fix potential integer overflow in `DDWAF_LOG_HELPER`.
- Add missing shared mingw64 build.
- Add example tool to run the WAF on a single rule with multiple test vectors.

### v1.0.16 (unstable) - 2021/12/15
- Fix duplicate matches in output ([#36](https://github.com/DataDog/libddwaf/issues/36))

### v1.0.15 (unstable) - 2021/12/07
- Support `min_length` option on `regex_match` operator.
- Remove `DDWAF_ERR_TIMEOUT` and update value of other errors.
- Add timeout field to `ddwaf_result`.
- Remove action field from `ddwaf_result`.
- Support MacOS 10.9.
- Minor CMake compatibility improvements.

### v1.0.14 (unstable) - 2021/10/26
- WAF output now conforms to the appsec event format v1.0.0.
- Add schema for output validation.
- Remove zip package generation.
- Minor improvements.

### v1.0.13 (unstable) - 2021/10/11
- Add support for ruleset format v2.1.
- Update fuzzer.
- Fix addresses with key path missing from ddwaf\_required\_addresses.
- Improve ruleset parsing logging.

### v1.0.12 (unstable) - 2021/10/01
- Add libinjection SQL and XSS rule processors.
- Add support for ruleset format v1.1 (adding is\_sqli and is\_xss operators).
- Improved universal x86\_64 and arm64 builds.
- Added darwin arm64 build.
- Fixed error on corpus generator for fuzzer.

### v1.0.11 (unstable) - 2021/09/16
- Improve contributor onboarding and readme.
- Cross-compile aarch64 static/shared libraries.
- Improve corpus generator for fuzzer.

### v1.0.10 (unstable) - 2021/09/13
- Add license to nuget package.

### v1.0.9 (unstable) - 2021/09/13
- Renamed static windows library to `ddwaf_static`.
- Correctly publish DSO dependencies.
- Add license and notice.
- Add copyright note to source files.
- Add issue and pull-request templates.

### v1.0.8 (unstable) - 2021/09/07
- Removed spdlog dependency.
- Fixed crash on base64encode transformer.
- Fixed crash on compressWhiteSpace transformer.
- Updated and fixed fuzzer.
- Fixed missing static library on windows packages.
- Other minor fixes and improvements.

### v1.0.7 (unstable) - 2021/08/31
- Support for new rule format, using `ddwaf::object`.
- Interface updated with `ddwaf` namespace.
- Removed pass-by-value and return-by-value from interface.
- Removed WAF singleton interface.
- Simplified WAF interface to be handle based and always additive.
- Clarified the ownership of `ddwaf::object` passed to the WAF.
- Removed functionality not supported by the new rule format.
- Added exception catch-all on interface functions to prevent std::terminate.

### v1.0.6 - 2020/10/23
- Convert integers to strings at the input of the WAF
- Report the manifest key of the parameter that we matched in the trigger report
- Fix a bug where we could send reports from a previously reported attack in follow-up executions of the additive API

### v1.0.5 - 2020/10/13
- Fix behavior of @exist on empty list
- Improve the cache bypass logic to only bypass it once per run
- Fix the cache overwrite logic when the bypass resulted in a match

### v1.0.4 - 2020/10/01
- Fix an issue where we wouldn't run on keys if the associtated value was a container in specific encapsulated containers
- Introduce a `numerize` transformer to better handle `Content-Length`

### v1.0.3 - 2020/09/29
- Fix an issue where we wouldn't run on keys if the associtated value was a container

### v1.0.2 - 2020/09/25
- Fix an issue where reports would be generated when no action is triggered
- Fix an issue where only the last step of a flow will trigger a report
- Fix an issue where reports would be incomplete if some rules triggered in previous run of the additive API

### v1.0.1 - 2020/09/23
- Fix a bug where we wouldn't run on keys if the associated value was shorter than a rule's options.min_length

### v1.0 - 2020/08/28
- Introduce transformers to extract CRS targets from the raw URI
- Introduce `removeComments` transformer
- Introduce `@ipMatch` operator

### v0.9.1 (1.0 preview 2) - 2020/08/24
- Introduce modifiers for a rule execution
- Introduce `@exist` operator
- Improve performance of the Additive API
- Reduce the frequency of perf cap check
- Return the detailed performance of the slowest rules
- Introduce allocation helpers
- Other performance optimisations

### v0.9.0 (1.0 preview) - 2020/08/10
- Introduce Additive API
- Introduce expanded initialization format
- Introduce Handle API
- Report performance metrics on each run
- Report the runtime of the slowest rules of each run
- Report the path of a match
- Introduce new transformers
- Rename and shorten the API names
- More...

### v0.7.0 - 2020/06/19
- Fix false positives in libinjection SQL heuristics
- Fix a false positive in libinjection XSS heuristics

### v0.6.1 - 2020/04/03
- When running a rule with multiple parameters, don't stop processing if a parameter is missing
- Add support for the `config` key in the init payload
- Add support for prefixes to operators
- Add a switch through both means to revert the first fix

### v0.6.0 - 2020/03/19
- Replace the clock we were using with a more efficient one
- When processing a multi step rule where a parameter is missing to one step, fail the step instead of ignoring it

### v0.5.1 - 2020/01/10
- Fix a bug where the Compare operators could read one byte after the end of a PWArgs buffer
- Fix a bug where lib injection might read one byte past an internal buffer

### v0.5.0 - 2019/11/15
- Give more control over the safety features to the API

### v0.4.0 - 2019/10/02
- Introduce `@pm` operator

### v0.3.0 - 2019/09/24
- Introduce `@beginsWith`, `@contains`, and `@endsWith` operators
- Cap the memory each RE2 object can use to 512kB

### v0.2.0 - 2019/09/13
- Introduce `powerwaf_initializePowerWAFWithDiag`
- Fix a UTF-8 trucation bug (SQR-8164)
- Cleanup headers
- Improved locking performance

### v0.1.0
- Initial release
