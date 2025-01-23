function post_draw()
    renderer:display_text("score: " .. tostring(game:read_int(game:read_int(0x00))), "r-type-offline/assets/font.ttf", 24, self:get_pos("x"), self:get_pos("y"), self:get_size("x"), self:get_size("y"));
end
