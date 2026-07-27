# List of assets to include and convert.
# Paths should be relative to res/graphics/.
# Please keep this list sorted in alphabetical order.
PNG2ASSET_ASSETS := \
    backgrounds/menu_background \
    fonts/orochi_jp_16x16 \
    fonts/yarara_font_8x8

# Per-asset png2asset flags, keyed by path relative to res/graphics/.
# Add one line per .ase file that needs conversion.
# Please keep this list sorted in alphabetical order.
PNG2ASSET_FLAGS/backgrounds/menu_background := -map -use_map_attributes
PNG2ASSET_FLAGS/fonts/orochi_jp_16x16 := -spr8x16 -sw 16 -sh 16
PNG2ASSET_FLAGS/fonts/yarara_font_8x8 := -spr8x8 -sw 8 -sh 8 -sprite_no_optimize
