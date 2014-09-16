pushd DSGMLib
make
pause
popd
pushd examples
pushd RoomPersistency
make
rem start "" "RoomPersistency.nds"
popd
pushd Collision
make
rem start "" "Collision.nds"
popd
pushd Priority
make
rem start "" "Priority.nds"
popd
pushd CustomVariables
make
rem start "" "CustomVariables.nds"
popd
pushd DrawableBackground
make
rem start "" "DrawableBackground.nds"
popd
pushd RotsetEffects
make
rem start "" "RotsetEffects.nds"
popd
pushd TopDownShooter
make
rem start "" "TopDownShooter.nds"
popd
popd