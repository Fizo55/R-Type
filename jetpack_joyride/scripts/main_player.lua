function move_player()
    playerx = self:get_pos("x")
    playery = self:get_pos("y")

    if playery > 800 then
        playery = playery - 1
    end
    if playery <= 800 then
        playery = playery + 1
    end
    self:move(playerx, playery)
end
