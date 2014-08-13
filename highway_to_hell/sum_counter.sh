#!/usr/bin/env bash
sh mark_counter.sh | paste -sd+ | bc

