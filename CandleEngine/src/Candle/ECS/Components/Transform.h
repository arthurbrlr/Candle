#pragma once

#include "Candle/CandleCore.h"
#include "Candle/ECS/Component.h"

namespace Candle {

	class Transform : public Component {

		public:
			Transform(Blueprint* parent, glm::vec3 position = { 0, 0, 0 }, glm::vec3 rotation = { 0, 0, 0 }, glm::vec3 scale = { 1, 1, 1 }); /* Constructor */
			void OnEditor() override; /* Used by the engine editor to display informations about the component */

				// Methods
			void Move(const glm::vec3& deltaMovement);	/* Move the transform in 3D space by an amount specified in the given vec3 */
			void Rotate(const glm::vec3& deltaRotate);	/* Rotate the transform in 3D space by an amount specified in the given vec3 */
			void Scale(const glm::vec3& deltaScale);	/* Scale the transform in 3D space with a different value for each axis, warning: a value of 1.1 will scale the transform in the direction by 110% */
			void Scale(const float deltaScale);			/* Scqle the transform in 3D space evenly */

			void ForceTransformUpdate();				/* Method used by other parent transforms to update the transform matrix */

				// Getters 
			const glm::mat4& Get() const;				/* Return the current transform matrix */ 
			const glm::vec3& GetPosition() const;		/* Return the current relative position */ 
			const glm::vec3& GetRotation() const;		/* Return the current relative rotation */ 
			const glm::vec3& GetScale() const;			/* Return the current relative scale */ 
			const glm::vec3 GetForward() const;			/* Return the forward vector of the transform */
			const glm::vec3 GetRight() const;			/* Return the right vector of the transform */

				// Setters
			Transform& SetPosition(const glm::vec3 newPosition);	/* Set the relative position */
			Transform& SetRotation(const glm::vec3 newRotation);	/* Set the relative rotation */
			Transform& SetScale(const glm::vec3 newScale);			/* Set the relative scale */

		private:
			glm::vec3 _position, _scale, _rotation;
			glm::mat4 _transformMatrix = glm::mat4(0);

			void UpdateMatrix();
	};

}