Player = Script {
	onCreate = function(self)
		self.body = self.node:findComponentByType("RigidBody"):asRigidBody()
		--self.node.color.r = 255
		--self.node.color.g = 0
		--self.node.color.b = 0
	end,
	onUpdate = function(self, dt)
		local vel = self.body:getLinearVelocity()

		local desiredVel = 0;
		if Input.isKeyPressed(68) then
			self.node.flipX = false
			desiredVel = 5
		elseif Input.isKeyPressed(65) then
			desiredVel = -5
			self.node.flipX = true
		end

		local velChange = desiredVel - vel.x
		local impulse = velChange * self.body:getMass()
		self.body:applyLinearImpulse(Vec2.new(impulse, 0), self.body:getWorldCenter(), true)

		velChange = 7 - vel.y
		impulse = velChange * self.body:getMass()
		if Input.isKeyPressed(87) and self.body:isOnFloor() then
			self.body:applyLinearImpulse(Vec2.new(0, impulse), self.body:getWorldCenter(), true)
		end

		if Input.isKeyPressed(90) then
			self.node.zOrder = self.node.zOrder + 1
		elseif Input.isKeyPressed(67) then
			self.node.zOrder = self.node.zOrder - 1
		end
	end
}