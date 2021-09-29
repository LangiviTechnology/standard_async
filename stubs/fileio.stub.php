<?php

/** @generate-function-entries */

function use_promise(): bool {}

function setTimeout(callable $callback, int $timout): int {}
function clearTimeout(int $timerId): bool {}

function setInterval(callable $callback, int $interval): int {}
function clearInterval(int $timerId): bool {}

function file_get_contents_async(string $filename, callable $cb, bool $use_include_path = false, $zcontext = null, int $offset = 0, int $maxlen = null): bool