function move_player()
    -- playerx = self:get_pos("x")
    -- playery = self:get_pos("y")

    -- if playery > 800 then
    --     playery = playery - 1
    -- end
    -- if playery <= 800 then
    --     playery = playery + 1
    -- end
    -- self:move(playerx, playery)
end

function move_up()
    playerx = self:get_pos("x")
    playery = self:get_pos("y")

    if playery <= 10 then
        playery = playery
    else
        playery = playery - 10
    end
    self:move(playerx, playery)
end

function move_down()
    playerx = self:get_pos("x")
    playery = self:get_pos("y")

    if playery > 700 then
        playery = playery
    else
        playery = playery + 10
    end
    self:move(playerx, playery)
end
