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
popd