#pragma once
#include <map>
#include <vector>
#include "../Util/Math.h"
#include "../Util/Curve.h"

namespace K3D12 {
	class VMDLoader;

#pragma pack(push,1)
	struct VMDHeader {
		//VMD�̃w�b�_���
		char vmdHeader[30];
		char VmdModelName[20];
	};
#pragma pack(pop)

#pragma pack(push,1)
	struct VMDMotion
	{
		//
		friend class  VMDLoader;
		//

		char boneName[15];
		unsigned int frameNo;
		Vector3 location; //position
		Quaternion rotation; //quaternion
		unsigned char interPolation[64];		//Completion
	};
#pragma pack(pop)
	//���[�V�������
	struct MotionKeyFrameData {
		unsigned int frameNo;
		Vector3	   location;
		Quaternion quaternion;

		Vector4 interpolationA;
		Vector4 interpolationB;

		Curve2D curve;

		MotionKeyFrameData(unsigned int frameNo, Quaternion quaternion, Vector3 location) :
			frameNo(frameNo), quaternion(quaternion), location(location) {};
		MotionKeyFrameData() :frameNo(0), quaternion() {};
	};

	struct MotionData {
		//�G�C���A�X
		unsigned int frame;													//���݂̃t���[��
		std::map<std::string, std::vector<MotionKeyFrameData>> keyFrame;	//�{�[�����ƃ��[�V�����f�[�^�̃y�A�z��
		std::vector<VMDMotion> vmdMotion;
	};

}