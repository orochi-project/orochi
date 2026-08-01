# List of assets to include and convert.
# Paths should be relative to res/audio/.
# Please keep this list sorted in alphabetical order.
UGE2SOURCE_ASSETS := \
    soundtrack/levels/01_komorebi/komorebi \
    soundtrack/levels/02_dream_flower/dream_flower \
    soundtrack/levels/03_senbonzakura/senbonzakura \
    soundtrack/levels/04_isolation/isolation \
    soundtrack/levels/05_freedom_dive/freedom_dive

# Per-asset uge2source bank assignments, keyed by path relative to res/audio/.
# Add one line per .uge file that needs assignment.
# Please keep this list sorted in alphabetical order.
UGE2SOURCE_BANK/soundtrack/levels/01_komorebi/komorebi := 2
UGE2SOURCE_BANK/soundtrack/levels/02_dream_flower/dream_flower := 3
UGE2SOURCE_BANK/soundtrack/levels/03_senbonzakura/senbonzakura := 4
UGE2SOURCE_BANK/soundtrack/levels/04_isolation/isolation := 5
UGE2SOURCE_BANK/soundtrack/levels/05_freedom_dive/freedom_dive := 6
