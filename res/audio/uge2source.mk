# List of assets to include and convert.
# Paths should be relative to res/audio/.
# Please keep this list sorted in alphabetical order.
UGE2SOURCE_ASSETS := \
    soundtrack/levels/01_komorebi/komorebi \
    soundtrack/levels/02_dream_flower/dream_flower \
    soundtrack/levels/03_thirdboot/thirdboot \
    soundtrack/levels/04_isolation/isolation \

# Per-asset uge2source bank assignments, keyed by path relative to res/audio/.
# Add one line per .uge file that needs assignment.
# Please keep this list sorted in alphabetical order.
UGE2SOURCE_BANK/soundtrack/levels/01_komorebi/komorebi := 2
UGE2SOURCE_BANK/soundtrack/levels/02_dream_flower/dream_flower := 3
UGE2SOURCE_BANK/soundtrack/levels/03_thirdboot/thirdboot := 4
UGE2SOURCE_BANK/soundtrack/levels/04_isolation/isolation := 5
