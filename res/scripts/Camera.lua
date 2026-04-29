
self.VecForward = {0,0,0}

function OnInit()
    print("Camera Initializing")


end


function OnUpdate()
    local vecForward = self:GetLocalForward()
    print("VecForward UPDATE cam = ".. vecForward.x..", ".. vecForward.y..", ".. vecForward.z)
    self.VecForward = self:GetLocalForward()
end