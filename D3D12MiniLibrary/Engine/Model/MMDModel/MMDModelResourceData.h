#pragma once
#include <memory>
#include <vector>
#include <string>
#include "../../Util/Math.h"
#include "../PMX/PMXModel.h"
#include "../PMD/PMDModel.h"
#include "../../PipelineState/PipelineStateObject.h"
#include "../../DescriptorHeap/DescriptorHeap.h"


namespace K3D12 {

	class ModelConverter;

	enum class MMDWeightDeformType {
		BDEF1,
		BDEF2,
		BDEF4,
		SDEF,
		QDEF
	};

	enum class MMDMaterialFlags {
		NO_CULL,
		GROUND_SHADOW,
		DRAW_SHADOW,
		RECEIVE_SHADOW,
		HAS_EDGE,
		VERTEX_COLOR,
		POINT_DRAWING,
		LINE_DRAWING
	};

	enum class MMDBoneFlags {
		INDEXED_TAIL_POSITION,
		ROTATABLE,
		TRANSLATABLE,
		IS_VISIBLE,
		ENABLE,
		IK,
		INHERIT_ROTATION,
		INHERIT_TRANSLATION,
		FIXED_AXIS,
		LOCAL_COORDINATE,
		PHISICS_AFTER_DEFORM,
		EXTERNAL_PARENT_DEFORM

	};

	enum BONE_OPTIONS {
		BONE_OPTION_CONNECTION = 0x0001, //: �ڑ���(PMD�q�{�[���w��)�\�����@ -> 0 : ���W�I�t�Z�b�g�Ŏw�� 1 : �{�[���Ŏw��
		BONE_OPTIONS_ROTATABLE = 0x0002, //: ��]�\
		BONE_OPTIONS_MOVABLE = 0x0004, //: �ړ��\
		BONE_OPTIONS_DISPLAY = 0x0008, //: �\��
		BONE_OPTIONS_CONTROLLABLE = 0x0010, //: �����
		BONE_OPTIONS_IK = 0x0020, //: IK
		BONE_OPTIONS_GRANTL_LOCAL = 0x0080, //: ���[�J���t�^ | �t�^�Ώ� 0 : ���[�U�[�ό`�l�^IK�����N�^���d�t�^ 1 : �e�̃��[�J���ό`��
		BONE_OPTIONS_GRANT_ROTATE = 0x0100, //: ��]�t�^
		BONE_OPTIONS_GRANT_MOVE = 0x0200, //: �ړ��t�^
		BONE_OPTIONS_AXIS_FIXING = 0x0400, //: ���Œ�
		BONE_OPTIONS_AXIS_LOCAL = 0x0800, //: ���[�J����
		BONE_OPTIONS_AFTER_PHYSICS = 0x1000, //: ������ό`
		BONE_OPTIONS_EXTERNAL_PARENT = 0x2000 //: �O���e�ό`
	};

	enum class MMDSphereBlendType {
		NONE,
		MULT,
		ADD,
		SUB_TEXURE
	};

	//�I���W�i���f�[�^��ҏW�i�ϊ��j���āA���̏��̕K�v���������̍\���̂ɑ���@���@�߂����Ⴈ�������@���@�񓯊��Ȃ�ǂ����낤��
	//�����_�����O�ɕK�v�Ȓ��_���(�ϊ��ςݒ��_���j
	struct MMDVertex {
		Vector3 pos;
		Vector3 normal;
		Vector2 texCoord;
		MMDWeightDeformType deformType;
		WeightDeform deformation;
		MMDVertex() : pos(), normal(), texCoord(), deformType(), deformation() {};
		~MMDVertex() {};
		MMDVertex(const MMDVertex& other) {
			*this = other;
		}
		MMDVertex& operator= (const MMDVertex& rhs) {
			pos = rhs.pos;
			normal = rhs.normal;
			texCoord = rhs.texCoord;
			deformType = rhs.deformType;
			deformation = rhs.deformation;
			return *this;
		}
	};

	//�����_�����O��⏕����}�e���A�����
	struct MMDMaterial {
		//	�g�U���ˌ� w = alpha
		Vector4 diffuse;
		//�@���ʔ��ˌ�	w = pow
		Vector4 specular;
		//����
		Vector3 ambient;
		//���Ȕ����F
		Vector3 emissive;
		//�X�t�B�A�}�b�v�̃u�����f�B���O�^�C�v
		int	sphereBlendType;
		//�e�N�X�`���e�[�u���̃C���f�b�N�X
		unsigned int textureTableIndex;
		//�X�t�B�A�}�b�v�̃C���f�b�N�X
		unsigned int	sphereIndex;
		//LUT�̎Q�ƃe�[�u���ԍ�
		unsigned int	toonIndex;
		//���̃}�e���A���`��܂ł̃C���f�b�N�X�I�t�Z�b�g
		unsigned int	surfaceCount;
	};

	//�����_�����O���ɃZ�b�g����C���f�b�N�X���
	struct MMDIndexList {
		std::vector<unsigned int> list;
	};

	//�����_�����O���ɕK�v�ȃe�N�X�`�����
	struct MMDTextureHeap {
		std::weak_ptr<DescriptorHeap> texturesHeap;
	};

	struct MMDBoneNode {
		//�e�ւ̃C���f�b�N�X
		unsigned int parentIndex;
		//���g�̃C���f�b�N�X
		unsigned int index;
		//�{�[���̈ʒu
		Vector3	pos;
		//�����|�[�Y����̉�]�ʁi�l�����j
		Quaternion	rotation;
		unsigned short	boneOptionFlags;
		struct MMDBoneOption {
			//�p���{�[��
			InheritBone inheritBone;
			//���[�J����
			BoneFixedAxis fixidAxis;
			//�Œ莲
			BoneLocalCoordinate localAxis;
			//�O���e�C���f�b�N�X
			BoneExternalParent	externalParentBoneIndex;
			MMDBoneOption() 
			{
				inheritBone = {};
				fixidAxis = {};
				localAxis = {};
				externalParentBoneIndex = {};
			};
			~MMDBoneOption() {};
		}boneOption;
		//���̃{�[����e�Ƃ��ĎQ�Ƃ���{�[��
		std::vector<MMDBoneNode> childrenBone;
		void Discard() {
			for (auto& child : childrenBone) {
				child.Discard();
			}
			this->childrenBone.clear();
		};

		MMDBoneNode() {};
		~MMDBoneNode() {
		};
	};

	struct MMDIKLinkData {
		unsigned int boneIndex;
		bool hasLimit;		//�����l����l�̔��f�t���O
		IKAngleLimit limits;
	};

	struct MMDIKData {
		unsigned int boneIndex;
		unsigned int targetBoneIndex; //IK�{�[�����ŏ��ɃR�l�N�e�B���O����{�[���C���f�b�N�X
		int loopCount;
		float limitRadian;
		std::vector<MMDIKLinkData> ikLinks;
		MMDIKData() {};
		~MMDIKData() {
			ikLinks.clear();
		}
	};
	//�t���O�����e�[�V����������
	struct MMDBoneTree {
		unsigned int boneNum;
		MMDBoneNode											rootBone;
		std::vector<Matrix>									bonesMatrix;
		std::vector<MMDIKData>								modelIKdata;
		std::map<std::string, MMDBoneNode*>					boneAccessor;
		std::map<unsigned int, std::string>					boneNameAccessor;
		MMDBoneTree() {

		};

		~MMDBoneTree() {
			bonesMatrix.clear();
			modelIKdata.clear();
			boneAccessor.clear();
		};
	};

	//MMD�œǂݍ��߂郂�f���f�[�^�iPMX�EPMD�j�̃I���W�i���f�[�^
	struct MMDModelResourceData
	{
		//���_���
		std::vector<MMDVertex>						_vertexes;
		//���_���
		std::vector<Vertex3D>						_originVert;
		//���_�C���f�b�N�X���
		MMDIndexList								_indexList;
		//�e�N�X�`�����
		MMDTextureHeap								_textureHeaps;
		//�}�e���A�����
		std::vector<MMDMaterial>					_materials;
		//�X�P���g��
		std::shared_ptr<MMDBoneTree>				_boneTree;
		//�e�N�X�`���p�X
		std::vector<std::string>					_texturePaths;
		//���f���p�X
		std::string									_modelPath;
		MMDModelResourceData();
		~MMDModelResourceData();
	};
}