# Food Order & Revenue Management

A C console application for menu management, order intake, delivery processing, and revenue reporting using linked lists and file persistence.

> **Kısa Türkçe özet:** Menü, sipariş, teslimat ve gelir akışını bağlı listeler ile yöneten C konsol uygulaması.

## Scope

- Add, list, and remove menu items.
- Create orders from existing menu items.
- Process queued orders and calculate revenue.
- Persist menu, orders, and revenue records in `data/`.

## Build

```bash
gcc -std=c11 -Wall -Wextra -pedantic src/main.c -o food-order-system
./food-order-system
```

Run from the repository root so the application can read the `data/` directory.

## Notes

This is an academic data-structures project. The uploaded version keeps the original linked-list and file-I/O design while adding safer string limits, missing-file handling, and invalid menu-item checks.

## Tech

C11, linked lists, file I/O
